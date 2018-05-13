#include "pch.h"
#include "CurveEditorSelectionTool.h"
#include "Components/SplinesComponent.h"
#include "Components/SelectionComponent.h"

CCurveEditorSelectionTool::CCurveEditorSelectionTool(ECurveEditorMouseButton activationMouseButton, ECurveEditorModifier togglingModifier) :
    CCurveEditorSelectionToolBase(activationMouseButton),
    m_TogglingModifier(togglingModifier)
{
}

void CCurveEditorSelectionTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    const auto& editorView = event.GetEditorView();

    m_SplinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(editorView);
    m_SelectionViewComponent = GetViewComponent<ICurveEditorSelectionViewComponent>(editorView);

    EDITOR_ASSERT(!m_SplinesViewComponent.expired());
    EDITOR_ASSERT(!m_SelectionViewComponent.expired());
}

void CCurveEditorSelectionTool::OnReleased(const CCurveEditorToolEvent&)
{
    m_SplinesViewComponent.reset();
    m_SelectionViewComponent.reset();
}

void CCurveEditorSelectionTool::OnClickUp(const CCurveEditorToolMouseButtonEvent& event)
{
    if (!CheckActivationButton(event))
        return;

    const auto splineViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViewComponent);
    if (!splineViewComponent)
        return;

    const auto clickedSplineComponent = splineViewComponent->GetSplineComponentAt(event.GetMousePosition(), m_SelectionType);

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (!selectionViewComponent)
        return;

    if (!m_TogglingMode)
    {
        selectionViewComponent->ClearSelection();
        if (clickedSplineComponent)
        {
            const auto result = selectionViewComponent->AddToSelection({ clickedSplineComponent });
            EDITOR_ASSERT(result);
        }
    }
    else if (clickedSplineComponent)
    {
        if (selectionViewComponent->CheckIfSelected(*clickedSplineComponent))
        {
            const auto result = selectionViewComponent->RemoveFromSelection({ clickedSplineComponent });
            EDITOR_ASSERT(result);
        }
        else
        {
            const auto result = selectionViewComponent->AddToSelection({ clickedSplineComponent });
            EDITOR_ASSERT(result);
        }
    }
}

void CCurveEditorSelectionTool::OnModifierActivated(const CCurveEditorToolModifierEvent& event)
{
    if (event.GetModifier() == m_TogglingModifier)
        m_TogglingMode = true;
}

void CCurveEditorSelectionTool::OnModifierDeactivated(const CCurveEditorToolModifierEvent& event)
{
    if (event.GetModifier() == m_TogglingModifier)
        m_TogglingMode = false;
}

bool CCurveEditorSelectionTool::AcceptSelection(const CCurveEditorToolMouseButtonEvent& event)
{
    const auto splineViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViewComponent);
    if (!splineViewComponent)
        return false;

    return splineViewComponent->GetSplineComponentAt(event.GetMousePosition()) == nullptr;
}

void CCurveEditorSelectionTool::OnSelectionBegin(ICurveEditorView&)
{
    m_TogglingSelection = m_TogglingMode;
    if (m_TogglingSelection)
        return;

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (selectionViewComponent)
        selectionViewComponent->ClearSelection();
}

void CCurveEditorSelectionTool::OnSelectionUpdate(ICurveEditorView& editorView, const ax::rectf& selectionRect)
{
    const auto splineViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViewComponent);
    if (!splineViewComponent)
        return;

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (!selectionViewComponent)
        return;

    const auto& editorStyle = editorView.GetEditorStyle();

    decltype(m_LastSelectedSplineComponents) componentsInRect;
    const auto visitor = [this, &componentsInRect, &selectionViewComponent](auto& splineComponent)
    {
        if (m_TogglingSelection)
        {
            if (selectionViewComponent->CheckIfSelected(splineComponent) &&
                m_LastSelectedSplineComponents.find(&splineComponent) == m_LastSelectedSplineComponents.cend())
                return;
        }

        componentsInRect.emplace(&splineComponent);
    };

    splineViewComponent->VisitSplineComponentsInRect(visitor, selectionRect, m_SelectionType, editorStyle.SelectionViaIntersection);

    if (m_LastSelectedSplineComponents == componentsInRect)
        return;

    decltype(componentsInRect) componentsToDeselect;
    std::set_difference(m_LastSelectedSplineComponents.begin(), m_LastSelectedSplineComponents.end(), componentsInRect.begin(), componentsInRect.end(), std::inserter(componentsToDeselect, componentsToDeselect.end()));

    if (!componentsToDeselect.empty())
    {
        const auto deselectionResult = selectionViewComponent->RemoveFromSelection(componentsToDeselect);
        EDITOR_ASSERT(deselectionResult);
    }

    decltype(componentsInRect) componentsToSelect;
    std::set_difference(componentsInRect.cbegin(), componentsInRect.cend(), m_LastSelectedSplineComponents.cbegin(), m_LastSelectedSplineComponents.cend(), std::inserter(componentsToSelect, componentsToSelect.end()));

    if (!componentsToSelect.empty())
    {
        const auto selectionResult = selectionViewComponent->AddToSelection(componentsToSelect);
        EDITOR_ASSERT(selectionResult);
    }

    m_LastSelectedSplineComponents = std::move(componentsInRect);
}

void CCurveEditorSelectionTool::OnSelectionEnd(ICurveEditorView&)
{
    m_LastSelectedSplineComponents.clear();
}