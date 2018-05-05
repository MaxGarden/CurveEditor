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

void CCurveEditorSelectionTool::OnClick(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != GetActivationMouseButton())
        return;

    const auto splineViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViewComponent);
    if (!splineViewComponent)
        return;

    const auto clickedSplineComponent = splineViewComponent->GetSplineComponentAt(event.GetMousePosition(), ECurveEditorSplineComponentType::Knot);

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (!selectionViewComponent)
        return;

    const auto selectionResult = [this, &selectionViewComponent, &clickedSplineComponent]()
    {
        if (!m_TogglingMode)
        {
            selectionViewComponent->ClearSelection();
            if (clickedSplineComponent)
                return selectionViewComponent->AddToSelection(*clickedSplineComponent);
        }
        else if (clickedSplineComponent)
        {
            if (selectionViewComponent->CheckIfSelected(*clickedSplineComponent))
                return selectionViewComponent->RemoveFromSelection(*clickedSplineComponent);
            else
                return selectionViewComponent->AddToSelection(*clickedSplineComponent);
        }

        return true;
    }();

    EDITOR_ASSERT(selectionResult);
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

void CCurveEditorSelectionTool::OnSelectionUpdate(ICurveEditorView& editorView, const ax::rectf& selectionRect)
{
    const auto splineViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViewComponent);
    if (!splineViewComponent)
        return;

    const auto& editorStyle = editorView.GetEditorStyle();

    decltype(m_LastSelectedSplineComponents) componentsInRect;
    const auto visitor = [&componentsInRect](auto& splineComponent)
    {
        componentsInRect.emplace(&splineComponent);
    };

    splineViewComponent->VisitSplineComponentsInRect(visitor, selectionRect, ECurveEditorSplineComponentType::Knot, editorStyle.SelectionViaIntersection);

    if (m_LastSelectedSplineComponents == componentsInRect)
        return;

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (!selectionViewComponent)
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