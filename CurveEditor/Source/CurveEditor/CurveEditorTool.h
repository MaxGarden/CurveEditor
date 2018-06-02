#pragma once
#if !defined(__CURVE_EDITOR_TOOL_H__)

#include "EditorTool.h"
#include "CurveEditorToolEvent.h"

class ICurveEditorTool : public IEditorTool
{
public:
    virtual ~ICurveEditorTool() override = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) = 0;
    virtual void OnReleased(const CCurveEditorToolEvent& event) = 0;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) = 0;
    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) = 0;
    virtual void OnDragEnd(const CCurveEditorToolMouseButtonEvent& event) = 0;

    virtual void OnMouseMove(const CCurveEditorToolMouseEvent& event) = 0;

    virtual void OnWheel(const CCurveEditorToolMouseWheelEvent& event) = 0;

    virtual void OnClickDown(const CCurveEditorToolMouseButtonEvent& event) = 0;
    virtual void OnClickUp(const CCurveEditorToolMouseButtonEvent& event) = 0;

    virtual void OnModifierActivated(const CCurveEditorToolModifierEvent& event) = 0;
    virtual void OnModifierDeactivated(const CCurveEditorToolModifierEvent& event) = 0;
};

class ICurveEditorComponentTool : public ICurveEditorTool
{
public:
    virtual ~ICurveEditorComponentTool() override = default;

    virtual bool AddComponent(ICurveEditorToolUniquePtr&& component) = 0;

    static ICurveEditorComponentToolUniquePtr Create();
};

#endif //__CURVE_EDITOR_TOOL_H__