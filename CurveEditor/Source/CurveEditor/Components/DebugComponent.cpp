#include "pch.h"
#include "DebugComponent.h"
#include "CurveEditorController.h"

CCurveEditorDebugComponent::CCurveEditorDebugComponent(const CCurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView)
{
}

void CCurveEditorDebugComponent::OnFrame(ImDrawList&, CCurveEditorController& editorController)
{
    ImGui::Begin("Debug Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowPos({});
    ImGui::InputText("Spline name", m_SplineName.data(), m_SplineName.size());

    ImGui::ColorEdit4("Spline color", &m_SplineColor.Value.x);

    if (ImGui::Button("Add Spline"))
        editorController.CreateSpline(m_SplineName, static_cast<ImU32>(m_SplineColor));

    ImGui::End();
}