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

CCurveEditorToolMouseDragEvent::CCurveEditorToolMouseDragEvent(ICurveEditorView& editorView, const ax::pointf& mousePosition, ECurveEditorMouseButton mouseButton, const ax::pointf& currentDragDelta, const ax::pointf& totalDragDelta) :
    CCurveEditorToolMouseButtonEvent(editorView, mousePosition, mouseButton),
    m_CurrentDragDelta(currentDragDelta),
    m_TotalDragDelta(totalDragDelta),
    m_DragStartPosition(GetMousePosition() - m_TotalDragDelta)
{
}

const ax::pointf& CCurveEditorToolMouseDragEvent::GetCurrentDragDelta() const noexcept
{
    return m_CurrentDragDelta;
}

const ax::pointf& CCurveEditorToolMouseDragEvent::GetTotalDragDelta() const noexcept
{
    return m_TotalDragDelta;
}

const ax::pointf& CCurveEditorToolMouseDragEvent::GetDragStartPosition() const noexcept
{
    return m_DragStartPosition;
}

CCurveEditorToolKeyEvent::CCurveEditorToolKeyEvent(ICurveEditorView& editorView) :
    CCurveEditorToolEvent(editorView)
{
}