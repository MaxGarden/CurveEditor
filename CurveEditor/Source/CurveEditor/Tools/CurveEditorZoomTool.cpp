#include "pch.h"
#include "CurveEditorZoomTool.h"
#include "Components/NavigationComponent.h"

CCurveEditorZoomTool::CCurveEditorZoomTool(std::optional<ECurveEditorModifier> horizonalAxisLockModifier /*= std::nullopt*/, std::optional<ECurveEditorModifier> verticalAxisLockModifier /*= std::nullopt*/) :
    m_HorizontalAxisLockModifier(horizonalAxisLockModifier),
    m_VerticalAxisLockModifier(verticalAxisLockModifier)
{
}

void CCurveEditorZoomTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    m_NavigationComponent = GetViewComponent<ICurveEditorNavigationComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_NavigationComponent.expired());
}

void CCurveEditorZoomTool::OnReleased(const CCurveEditorToolEvent&)
{
    m_NavigationComponent.reset();
}

void CCurveEditorZoomTool::OnModifierActivated(const CCurveEditorToolModifierEvent& event)
{
    if (m_HorizontalAxisLockModifier && event.GetModifier() == *m_HorizontalAxisLockModifier)
        m_ZoomAxisMultiplier.x = 0.0f;
    else if (m_VerticalAxisLockModifier && event.GetModifier() == *m_VerticalAxisLockModifier)
        m_ZoomAxisMultiplier.y = 0.0f;
}

void CCurveEditorZoomTool::OnModifierDeactivated(const CCurveEditorToolModifierEvent& event)
{
    if (m_HorizontalAxisLockModifier && event.GetModifier() == *m_HorizontalAxisLockModifier)
        m_ZoomAxisMultiplier.x = 1.0f;
    else if (m_VerticalAxisLockModifier && event.GetModifier() == *m_VerticalAxisLockModifier)
        m_ZoomAxisMultiplier.y = 1.0f;
}

void CCurveEditorZoomTool::OnWheel(const CCurveEditorToolMouseWheelEvent& event)
{
    const auto navigationComponent = m_NavigationComponent.lock();
    EDITOR_ASSERT(navigationComponent);
    if (!navigationComponent)
        return;

    navigationComponent->ApplyZoom(event.GetMousePosition(), static_cast<int>(event.GetWheelValue()), m_ZoomAxisMultiplier);
}