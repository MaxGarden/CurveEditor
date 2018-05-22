#pragma  once
#if !defined(__CURVE_EDITOR_ZOOM_TOOL_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorZoomTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorZoomTool() = default;
    virtual ~CCurveEditorZoomTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnWheel(const CCurveEditorToolMouseWheelEvent& event) override final;

private:
    ICurveEditorNavigationComponentWeakPtr m_NavigationComponent;
};

#endif //__CURVE_EDITOR_ZOOM_TOOL_H__