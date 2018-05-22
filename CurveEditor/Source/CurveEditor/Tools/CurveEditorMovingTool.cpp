#include "pch.h"
#include "SplineView.h"
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

    const auto splinesViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splinesViewComponent);
    if (!splinesViewComponent)
        return;

    const auto draggingSplineComponentView = splinesViewComponent->GetSplineComponentAt(event.GetMousePosition());
    if (!draggingSplineComponentView)
        return;

    const auto result = AddSplineComponentViewToDrag(*draggingSplineComponentView, *splinesViewComponent);
    EDITOR_ASSERT(result);
    if (!result)
        return;

    CScopedGuard saveStateGuard{ [this]()
    {
        VisitPointersContainer(m_DraggingSplines, [](auto& splineView)
        {
            splineView.SaveState();
        });
    } };

    const auto selectionViewComponent = m_SelectionViewComponent.lock();
    EDITOR_ASSERT(selectionViewComponent);
    if (!selectionViewComponent)
        return;

    if (selectionViewComponent->GetSelectionMode() != draggingSplineComponentView->GetType())
        return;

    selectionViewComponent->VisitSelection([this, &splinesViewComponent](auto& splineComponentView)
    {
        const auto result = AddSplineComponentViewToDrag(splineComponentView, *splinesViewComponent);
        EDITOR_ASSERT(result);
    });
}

void CCurveEditorMovingTool::OnDragUpdate(const CCurveEditorToolMouseDragEvent& event)
{
    if (m_DraggingSplineComponentsViews.empty() || event.GetMouseButton() != m_ActivationMouseButton)
        return;

    const auto& editorCanvas = event.GetEditorView().GetCanvas();

    const auto& dragDelta = editorCanvas.FromEditor(event.GetDragDelta(), false);

    VisitPointersContainer(m_DraggingSplines, [](auto& splineView)
    {
        splineView.RestoreState();
    });

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

    VisitPointersContainer(m_DraggingSplines, [](auto& splineView)
    {
        splineView.ResetSavedState();
    });

    ResetDraggedSplineComponentsViews();
}

bool CCurveEditorMovingTool::AddSplineComponentViewToDrag(ICurveEditorSplineComponentView& splineComponentView, ICurveEditorSplinesViewComponent& splinesViewComponent)
{
    if (m_DraggingSplineComponentsViews.find(&splineComponentView) != m_DraggingSplineComponentsViews.cend())
        return true;

    const auto position = splineComponentView.GetPosition();
    EDITOR_ASSERT(position);
    if (!position)
        return false;

    const auto splineView = splinesViewComponent.GetSplineView(splineComponentView.GetSplineID());
    EDITOR_ASSERT(splineView);
    if (!splineView)
        return false;

    m_DraggingSplines.emplace(splineView).second;
    m_DraggingSplineComponentsViews.try_emplace(&splineComponentView, *position).second;

    return true;
}

void CCurveEditorMovingTool::ResetDraggedSplineComponentsViews()
{
    m_DraggingSplineComponentsViews.clear();
    m_DraggingSplines.clear();
}