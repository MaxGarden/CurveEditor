#include "pch.h"
#include "DebugComponent.h"
#include "CurveEditorController.h"

CCurveEditorDebugComponent::CCurveEditorDebugComponent(const CCurveEditorView& editorView) :
	CCurveEditorViewComponentBase(editorView)
{
}

void CCurveEditorDebugComponent::OnFrame(ImDrawList&, CCurveEditorController& controller)
{
	ImGui::Begin("Debug Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::InputText("Spline name", m_SplineName.data(), m_SplineName.size());

	if (ImGui::Button("Add Spline"))
		controller.CreateSpline(m_SplineName);

    ImGui::SameLine();

    if (ImGui::Button("Destroy Spline"))
        controller.DestroySpline(m_SplineName);

	ImGui::End();
}