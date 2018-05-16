#include "pch.h"
#include "CurveEditorSelectionTool.h"
#include "Components/SplinesComponent.h"
#include "Components/SelectionComponent.h"

CCurveEditorSelectionTool::CCurveEditorSelectionTool(ECurveEditorMouseButton activationMouseButton, ECurveEditorSplineComponentType defaultSelectionMode, std::optional<ECurveEditorModifier> togglingModifier, std::map<ECurveEditorModifier, ECurveEditorSplineComponentType> selectionModesMap) :
    CCurveEditorSelectionToolBase(activationMouseButton),
    m_TogglingModifier(togglingModifier),
    m_SelectionModesMap(std::move(selectionModesMap)),
    m_DefaultSelectionMode(defaultSelectionMode),
    m_CurrentSelectionMode(m_DefaultSelectionMode)
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

    const auto clickedSplineComponent = splineViewComponent->GetSplineComponentAt(event.GetMousePosition(), m_CurrentSelectionMode);

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (!selectionViewComponent)
        return;

    UpdateSelectionMode(*selectionViewComponent);

    if (!m_TogglingMode)
    {
        selectionViewComponent->ClearSelection();
        m_LastSelectedSplineComponents.clear();
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
    const auto& modifier = event.GetModifier();

    if (m_TogglingModifier && modifier == *m_TogglingModifier)
        m_TogglingMode = true;

    const auto iterator = m_SelectionModesMap.find(modifier);
    if (iterator == m_SelectionModesMap.cend())
        return;

    if (m_CurrentSelectionMode != m_DefaultSelectionMode)
        m_BufferedSelectionModes.emplace_back(m_CurrentSelectionMode);

    m_CurrentSelectionMode = iterator->second;

    OnModifiersChanged();
}

void CCurveEditorSelectionTool::OnModifierDeactivated(const CCurveEditorToolModifierEvent& event)
{
    const auto& modifier = event.GetModifier();

    if (m_TogglingModifier && modifier == *m_TogglingModifier)
        m_TogglingMode = false;

    const auto iterator = m_SelectionModesMap.find(modifier);
    if (iterator == m_SelectionModesMap.cend())
        return;

    if (m_CurrentSelectionMode == iterator->second)
    {
        if (m_BufferedSelectionModes.empty())
            m_CurrentSelectionMode = m_DefaultSelectionMode;
        else
        {
            m_CurrentSelectionMode = m_BufferedSelectionModes.back();
            m_BufferedSelectionModes.pop_back();
        }
    }
    else
        RemoveFromContainer(m_BufferedSelectionModes, iterator->second);

    OnModifiersChanged();
}

bool CCurveEditorSelectionTool::AcceptSelection(const CCurveEditorToolMouseButtonEvent& event)
{
    const auto splineViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViewComponent);
    if (!splineViewComponent)
        return false;

    return splineViewComponent->GetSplineComponentAt(event.GetMousePosition()) == nullptr;
}

void CCurveEditorSelectionTool::OnSelectionBegin(ICurveEditorView& editorView)
{
    m_TogglingSelection = m_TogglingMode;
    if (m_TogglingSelection)
        return;

    const auto& editorStyle = editorView.GetEditorStyle();
    m_SelectionViaIntersection = editorStyle.SelectionViaIntersection;

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (selectionViewComponent)
        selectionViewComponent->ClearSelection();
}

void CCurveEditorSelectionTool::OnSelectionUpdate(ICurveEditorView&, const ax::rectf& selectionRect)
{
    UpdateSelection(selectionRect);
}

void CCurveEditorSelectionTool::OnSelectionEnd(ICurveEditorView&)
{
    m_LastSelectedSplineComponents.clear();
}

void CCurveEditorSelectionTool::OnModifiersChanged()
{
    if (const auto selectionRect = GetSelectionRect())
        UpdateSelection(*selectionRect);
}

void CCurveEditorSelectionTool::UpdateSelectionMode(ICurveEditorSelectionViewComponent& selectionViewComponent)
{
    const auto selectionMode = selectionViewComponent.GetSelectionMode();
    EDITOR_ASSERT(selectionMode);

    if (selectionMode && *selectionMode != m_CurrentSelectionMode)
    {
        selectionViewComponent.SetSelectionMode(m_CurrentSelectionMode);
        m_LastSelectedSplineComponents.clear();
    }
}

void CCurveEditorSelectionTool::UpdateSelection(const ax::rectf& selectionRect)
{
    const auto splineViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViewComponent);
    if (!splineViewComponent)
        return;

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (!selectionViewComponent)
        return;

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

    splineViewComponent->VisitSplineComponentsInRect(visitor, selectionRect, m_CurrentSelectionMode, m_SelectionViaIntersection);

    if (m_LastSelectedSplineComponents == componentsInRect)
        return;

    UpdateSelectionMode(*selectionViewComponent);

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