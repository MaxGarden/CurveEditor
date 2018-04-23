#include "pch.h"
#include "DebugComponent.h"
#include "CurveEditorController.h"
#include "EditorContext.h"

CCurveEditorDebugComponent::CCurveEditorDebugComponent(ICurveEditorView& editorView, IEditorContext& editorContext) :
    CCurveEditorViewVisibleComponentBase(editorView),
    m_EditorContext(editorContext)
{
}

void CCurveEditorDebugComponent::OnFrame(ImDrawList&, ICurveEditorController&)
{
    ImGui::Begin("Debug Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::InputText("Spline name", m_SplineName.data(), m_SplineName.size());

    ImGui::ColorEdit4("Spline color", &m_SplineColor.Value.x);

    const auto getEditorDataModel = [this]() -> ICurveEditorDataModelSharedPtr
    {
        return std::dynamic_pointer_cast<ICurveEditorDataModel>(m_EditorContext.GetDataModel());
    };

    if (ImGui::Button("Add Spline"))
    {
        if (const auto editorDataModel = getEditorDataModel())
        {
            auto splineDataModel = editorDataModel->AddSplineDataModel(m_SplineName, static_cast<ImU32>(m_SplineColor));
            EDITOR_ASSERT(splineDataModel);
            m_CreatedSplinesDataModels.emplace(std::move(splineDataModel));
        }
    }

    if (!m_CreatedSplinesDataModels.empty() && ImGui::Button("Remove Spline"))
    {
        if (const auto editorDataModel = getEditorDataModel())
        {
            editorDataModel->RemoveSplineDataModel(m_CreatedSplinesDataModels.top().lock());
            m_CreatedSplinesDataModels.pop();
        }
    }

    ImGui::End();
}