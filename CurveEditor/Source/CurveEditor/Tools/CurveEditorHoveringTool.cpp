#include "pch.h"
#include "CurveEditorHoveringTool.h"
#include "Components/SplinesComponent.h"

void CCurveEditorHoveringTool::OnActiveEditorViewChanged(const CCurveEditorToolEvent& event)
{
    m_SplinesViewComponent = GetViewComponent<CCurveEditorSplinesViewComponent>(event.GetEditorView());
}

void CCurveEditorHoveringTool::OnMouseMove(const CCurveEditorToolMouseEvent& event)
{
    const auto splineViews = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViews);
    if (!splineViews)
        return;

}