#include "pch.h"
#include "CurveEditorToolEvent.h"

CCurveEditorToolEvent::CCurveEditorToolEvent(const CCurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

const CCurveEditorView& CCurveEditorToolEvent::GetEditorView() const noexcept
{
    return m_EditorView;
}

CCurveEditorToolWheelEvent::CCurveEditorToolWheelEvent(const CCurveEditorView& editorView) :
    CCurveEditorToolEvent(editorView)
{
}

CCurveEditorToolMouseEvent::CCurveEditorToolMouseEvent(const CCurveEditorView& editorView, const ax::pointf& mousePosition) :
    CCurveEditorToolEvent(editorView),
    m_MousePosition(mousePosition)
{
}

const ax::pointf& CCurveEditorToolMouseEvent::GetMousePosition() const noexcept
{
    return m_MousePosition;
}

CCurveEditorToolMouseButtonEvent::CCurveEditorToolMouseButtonEvent(const CCurveEditorView& editorView, const ax::pointf& mousePosition, ECurveEditorMouseButton mouseButton) :
    CCurveEditorToolMouseEvent(editorView, mousePosition),
    m_MouseButton(mouseButton)
{
}

ECurveEditorMouseButton CCurveEditorToolMouseButtonEvent::GetMouseButton() const noexcept
{
    return m_MouseButton;
}

CCurveEditorToolKeyEvent::CCurveEditorToolKeyEvent(const CCurveEditorView& editorView) :
    CCurveEditorToolEvent(editorView)
{
}