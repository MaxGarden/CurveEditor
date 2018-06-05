#include "pch.h"
#include "CurveEditorSplineRemoverTool.h"
#include "Components/SplinesComponent.h"

CCurveEditorSplineRemoverTool::CCurveEditorSplineRemoverTool(ECurveEditorMouseButton removeButton /* = ECurveEditorMouseButton::Left */) :
    m_RemoveButton(removeButton)
{
}

void CCurveEditorSplineRemoverTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    m_SpinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_SpinesViewComponent.expired());
}

void CCurveEditorSplineRemoverTool::OnReleased(const CCurveEditorToolEvent&)
{
    m_SpinesViewComponent.reset();
}

void CCurveEditorSplineRemoverTool::OnClickUp(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != m_RemoveButton)
        return;

    const auto splinesViewComponent = m_SpinesViewComponent.lock();
    EDITOR_ASSERT(splinesViewComponent);
    if (!splinesViewComponent)
        return;

    const auto splineViewComponent = splinesViewComponent->GetSplineComponentAt(event.GetMousePosition(), ECurveEditorSplineComponentType::Curve);
    if (!splineViewComponent)
        return;

    EDITOR_ASSERT(splineViewComponent->GetType() == ECurveEditorSplineComponentType::Curve);

    const auto result = splinesViewComponent->RemoveSpline(splineViewComponent->GetSplineID());
    EDITOR_ASSERT(result);
}