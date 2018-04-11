#include "pch.h"
#include "CurveEditorScrollTool.h"
#include "CurveEditorView.h"

CCurveEditorScrollTool::CCurveEditorScrollTool(ECurveEditorMouseButton mouseButton) :
    m_MouseButton(mouseButton)
{
}

void CCurveEditorScrollTool::OnDragBegin(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != m_MouseButton)
        return;

    m_LastDragDelta = {};
}

void CCurveEditorScrollTool::OnDragUpdate(const CCurveEditorToolMouseDragEvent& event)
{
    if (event.GetMouseButton() != m_MouseButton)
        return;

    auto& editorView = event.GetEditorView();
    auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto& dragDelta = event.GetDragDelta().cwise_product(windowCanvas.GetInvertZoom());

    if (dragDelta == m_LastDragDelta)
        return;

    windowCanvas.SetClientOrigin(windowCanvas.GetClientOrigin() + dragDelta - m_LastDragDelta);
    m_LastDragDelta = dragDelta;
}