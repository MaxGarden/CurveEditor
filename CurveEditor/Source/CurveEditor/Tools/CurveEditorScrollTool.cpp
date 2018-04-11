#include "pch.h"
#include "CurveEditorScrollTool.h"
#include "CurveEditorView.h"

CCurveEditorScrollTool::CCurveEditorScrollTool(ECurveEditorMouseButton mouseButton) :
    m_MouseButton(mouseButton)
{
}

void CCurveEditorScrollTool::OnDragUpdate(const CCurveEditorToolMouseDragEvent& event)
{
    if (event.GetMouseButton() != m_MouseButton)
        return;

    auto& editorView = event.GetEditorView();
    auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto& currentDragDelta = event.GetCurrentDragDelta();

    windowCanvas.SetClientOrigin(windowCanvas.GetClientOrigin() + currentDragDelta);
}