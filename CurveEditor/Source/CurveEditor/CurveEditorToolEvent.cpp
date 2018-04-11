#include "pch.h"
#include "CurveEditorToolEvent.h"

CCurveEditorToolEvent::CCurveEditorToolEvent(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

ICurveEditorView& CCurveEditorToolEvent::GetEditorView() const noexcept
{
    return m_EditorView;
}

CCurveEditorToolMouseWheelEvent::CCurveEditorToolMouseWheelEvent(ICurveEditorView& editorView, const ax::pointf& mousePosition, float wheelValue) :
    CCurveEditorToolMouseEvent(editorView, mousePosition),
    m_WheelValue(wheelValue)
{
}

float CCurveEditorToolMouseWheelEvent::GetWheelValue() const noexcept
{
    return m_WheelValue;
}

CCurveEditorToolMouseEvent::CCurveEditorToolMouseEvent(ICurveEditorView& editorView, const ax::pointf& mousePosition) :
    CCurveEditorToolEvent(editorView),
    m_MousePosition(mousePosition)
{
}

const ax::pointf& CCurveEditorToolMouseEvent::GetMousePosition() const noexcept
{
    return m_MousePosition;
}

CCurveEditorToolMouseButtonEvent::CCurveEditorToolMouseButtonEvent(ICurveEditorView& editorView, const ax::pointf& mousePosition, ECurveEditorMouseButton mouseButton) :
    CCurveEditorToolMouseEvent(editorView, mousePosition),
    m_MouseButton(mouseButton)
{
}

ECurveEditorMouseButton CCurveEditorToolMouseButtonEvent::GetMouseButton() const noexcept
{
    return m_MouseButton;
}


CCurveEditorToolMouseDragEvent::CCurveEditorToolMouseDragEvent(ICurveEditorView& editorView, const ax::pointf& mousePosition, ECurveEditorMouseButton mouseButton, const ax::pointf& dragDelta) :
    CCurveEditorToolMouseButtonEvent(editorView, mousePosition, mouseButton),
    m_DragDelta(dragDelta)
{
}

const ax::pointf& CCurveEditorToolMouseDragEvent::GetDragDelta() const noexcept
{
    return m_DragDelta;
}

CCurveEditorToolKeyEvent::CCurveEditorToolKeyEvent(ICurveEditorView& editorView) :
    CCurveEditorToolEvent(editorView)
{
}