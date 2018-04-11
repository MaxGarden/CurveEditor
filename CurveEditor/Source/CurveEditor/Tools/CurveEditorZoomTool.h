#pragma  once
#if !defined(__CURVE_EDITOR_ZOOM_TOOL_H__)

#include "CurveEditorToolBase.h"
class CCurveEditorZoomTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorZoomTool() = default;
    virtual ~CCurveEditorZoomTool() override final = default;

    virtual void OnWheel(const CCurveEditorToolMouseWheelEvent& event) override final;

private:
    void ApplyZoom(int steps, ICurveEditorView& editorView);
    void CalculateLevelFromZoom();

private:
    ax::pointf m_ZoomLevel;
};

#endif //__CURVE_EDITOR_ZOOM_TOOL_H__