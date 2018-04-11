#pragma  once
#if !defined(__CURVE_EDITOR_TOOL_BASE_H__)

#include "CurveEditorTool.h"

class CCurveEditorToolBase : public ICurveEditorTool
{
public:
    CCurveEditorToolBase() = default;
    virtual ~CCurveEditorToolBase() override = default;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) override;
    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) override;
    virtual void OnDragEnd(const CCurveEditorToolMouseButtonEvent& event) override;

    virtual void OnMouseMove(const CCurveEditorToolMouseEvent& event) override;

    virtual void OnWheel(const CCurveEditorToolMouseWheelEvent& event) override;

    virtual void OnClick(const CCurveEditorToolMouseButtonEvent& event) override;
    virtual void OnDoubleClick(const CCurveEditorToolMouseButtonEvent& event) override;

    virtual void OnKeyDown(const CCurveEditorToolKeyEvent& event) override;
    virtual void OnKeyUp(const CCurveEditorToolKeyEvent& event) override;
};

#endif //__CURVE_EDITOR_TOOL_BASE_H__