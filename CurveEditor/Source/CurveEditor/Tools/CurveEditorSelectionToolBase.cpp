#include "pch.h"
#include "CurveEditorSelectionToolBase.h"
#include "CurveEditorView.h"
#include "ImGuiInterop.h"

using namespace ax::ImGuiInterop;

CCurveEditorSelectionToolBase::CCurveEditorSelectionToolBase(ECurveEditorMouseButton mouseButton) :
    m_MouseButton(mouseButton)
{
}

void CCurveEditorSelectionToolBase::OnDragBegin(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != m_MouseButton)
        return;

    m_DragStartPosition = event.GetMousePosition();
}

void CCurveEditorSelectionToolBase::OnDragUpdate(const CCurveEditorToolMouseDragEvent& event)
{
    if (event.GetMouseButton() != m_MouseButton)
        return;

    auto& editorView = event.GetEditorView();
    auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto a = to_imvec(windowCanvas.ToScreen(m_DragStartPosition));
    const auto b = to_imvec(windowCanvas.ToScreen(event.GetMousePosition()));

    ImGui::GetWindowDrawList()->AddRectFilled(a, b, ImColor(1.0, 0.0f, 0.0f));
    ImGui::GetWindowDrawList()->AddCircleFilled(a, 5.0f, ImColor(0.0, 1.0f, 0.0f));
}