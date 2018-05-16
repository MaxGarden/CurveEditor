#include "pch.h"
#include "CurveEditorMovingTool.h"
#include "Components/SplinesComponent.h"
#include "Components/SelectionComponent.h"

CCurveEditorMovingTool::CCurveEditorMovingTool(ECurveEditorMouseButton activationMouseButton /* = ECurveEditorMouseButton::Left */) :
    m_ActivationMouseButton(activationMouseButton)
{
}

void CCurveEditorMovingTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    m_SplinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_SplinesViewComponent.expired());

    m_SelectionViewComponent = GetViewComponent<ICurveEditorSelectionViewComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_SelectionViewComponent.expired());
}

void CCurveEditorMovingTool::OnReleased(const CCurveEditorToolEvent&)
{
    m_SplinesViewComponent.reset();
    m_SelectionViewComponent.reset();
}

void CCurveEditorMovingTool::OnDragBegin(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != m_ActivationMouseButton)
        return;

    const auto splineViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViewComponent);
    if (!splineViewComponent)
        return;

    const auto addSplineCompnentViewToDrag = [this](auto& splineComponentView)
    {
        if (m_DraggingSplineComponentsViews.find(&splineComponentView) != m_DraggingSplineComponentsViews.cend())
            return;

        const auto startPosition = splineComponentView.GetPosition();
        EDITOR_ASSERT(startPosition);
        if (!startPosition)
            return;

        const auto result = m_DraggingSplineComponentsViews.try_emplace(&splineComponentView, *startPosition).second;
        EDITOR_ASSERT(result);
    };

    const auto draggingSplineComponentView = splineViewComponent->GetSplineComponentAt(event.GetMousePosition());
    if (!draggingSplineComponentView)
        return;

    addSplineCompnentViewToDrag(*draggingSplineComponentView);

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (!selectionViewComponent)
        return;

    if (selectionViewComponent->GetSelectionMode() != draggingSplineComponentView->GetType())
        return;

    selectionViewComponent->VisitSelection([&addSplineCompnentViewToDrag](auto& splineComponentView)
    {
        addSplineCompnentViewToDrag(splineComponentView);
    });
}

void CCurveEditorMovingTool::OnDragUpdate(const CCurveEditorToolMouseDragEvent& event)
{
    if (m_DraggingSplineComponentsViews.empty() || event.GetMouseButton() != m_ActivationMouseButton)
        return;

    const auto& editorCanvas = event.GetEditorView().GetCanvas();

    const auto& dragDelta = editorCanvas.FromEditor(event.GetDragDelta(), false);

    VisitObjectsContainer(m_DraggingSplineComponentsViews, [&dragDelta](const auto& pair)
    {
        const auto splineComponentView = pair.first;
        EDITOR_ASSERT(splineComponentView);
        if (!splineComponentView)
            return;

        const auto result = splineComponentView->SetPosition(pair.second + dragDelta);
        EDITOR_ASSERT(result);
    });
}

void CCurveEditorMovingTool::OnDragEnd(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != m_ActivationMouseButton)
        return;

    m_DraggingSplineComponentsViews.clear();
}