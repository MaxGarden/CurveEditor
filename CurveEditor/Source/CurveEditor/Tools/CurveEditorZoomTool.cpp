#include "pch.h"
#include "CurveEditorZoomTool.h"
#include "CurveEditorToolEvent.h"
#include "CurveEditorView.h"
#include "Components/NavigationComponent.h"

void CCurveEditorZoomTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    m_NavigationComponent = GetViewComponent<ICurveEditorNavigationComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_NavigationComponent.expired());
}

void CCurveEditorZoomTool::OnReleased(const CCurveEditorToolEvent&)
{
    m_NavigationComponent.reset();
}

void CCurveEditorZoomTool::OnWheel(const CCurveEditorToolMouseWheelEvent& event)
{
    const auto navigationComponent = m_NavigationComponent.lock();
    EDITOR_ASSERT(navigationComponent);
    if (!navigationComponent)
        return;

    navigationComponent->ApplyZoom(event.GetMousePosition(), static_cast<int>(event.GetWheelValue()));
}