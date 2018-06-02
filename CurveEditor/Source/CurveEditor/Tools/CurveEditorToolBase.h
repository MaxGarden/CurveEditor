#pragma once
#if !defined(__CURVE_EDITOR_TOOL_BASE_H__)

#include "CurveEditorTool.h"

class CCurveEditorToolBase : public ICurveEditorTool
{
public:
    CCurveEditorToolBase() = default;
    virtual ~CCurveEditorToolBase() override = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) override;
    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) override;
    virtual void OnDragEnd(const CCurveEditorToolMouseButtonEvent& event) override;

    virtual void OnMouseMove(const CCurveEditorToolMouseEvent& event) override;

    virtual void OnWheel(const CCurveEditorToolMouseWheelEvent& event) override;

    virtual void OnClickDown(const CCurveEditorToolMouseButtonEvent& event) override;
    virtual void OnClickUp(const CCurveEditorToolMouseButtonEvent& event) override;

    virtual void OnModifierActivated(const CCurveEditorToolModifierEvent& event) override;
    virtual void OnModifierDeactivated(const CCurveEditorToolModifierEvent& event) override;
};

#endif //__CURVE_EDITOR_TOOL_BASE_H__