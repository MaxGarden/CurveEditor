#include "pch.h"
#include "CurveEditorScrollTool.h"
#include "CurveEditorView.h"

CCurveEditorScrollTool::CCurveEditorScrollTool(ECurveEditorMouseButton activationMouseButton) :
    m_ActivationMouseButton(activationMouseButton)
{
}

void CCurveEditorScrollTool::OnDragBegin(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != m_ActivationMouseButton)
        return;

    m_LastDragDelta = {};
}

void CCurveEditorScrollTool::OnDragUpdate(const CCurveEditorToolMouseDragEvent& event)
{
    if (event.GetMouseButton() != m_ActivationMouseButton)
        return;

    auto& editorView = event.GetEditorView();
    auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto& dragDelta = event.GetDragDelta().cwise_product(windowCanvas.GetInvertZoom());

    if (dragDelta == m_LastDragDelta)
        return;

    windowCanvas.SetClientOrigin(windowCanvas.GetClientOrigin() + dragDelta - m_LastDragDelta);
    m_LastDragDelta = dragDelta;
}