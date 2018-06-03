#pragma once
#if !defined(__CURVE_EDITOR_ZOOM_TOOL_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorZoomTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorZoomTool(std::optional<ECurveEditorModifier> horizonalAxisLockModifier = std::nullopt, std::optional<ECurveEditorModifier> verticalAxisLockModifier = std::nullopt);
    virtual ~CCurveEditorZoomTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnModifierActivated(const CCurveEditorToolModifierEvent& event) override final;
    virtual void OnModifierDeactivated(const CCurveEditorToolModifierEvent& event) override final;

    virtual void OnWheel(const CCurveEditorToolMouseWheelEvent& event) override final;

private:
    const std::optional<ECurveEditorModifier> m_HorizontalAxisLockModifier;
    const std::optional<ECurveEditorModifier> m_VerticalAxisLockModifier;

    ICurveEditorNavigationComponentWeakPtr m_NavigationComponent;

    ax::pointf m_ZoomAxisMultiplier = { 1.0f, 1.0f };
};

#endif //__CURVE_EDITOR_ZOOM_TOOL_H__