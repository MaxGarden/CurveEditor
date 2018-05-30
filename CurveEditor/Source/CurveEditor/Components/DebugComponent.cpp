#include "pch.h"
#include "DebugComponent.h"
#include "CurveEditorViewVisibleComponentBase.h"
#include "CurveEditorController.h"
#include "EditorContext.h"

class CCurveEditorDebugComponent final : public CCurveEditorViewVisibleComponentBase<ICurveEditorDebugComponent>
{
public:
    CCurveEditorDebugComponent(ICurveEditorView& editorView, IEditorContext& editorContext);
    virtual ~CCurveEditorDebugComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorController& editorController) override final;

private:
    IEditorContext & m_EditorContext;
    ImColor m_SplineColor = ImColor(0.0f, 1.0f, 0.0f);

    std::stack<ICurveEditorSplineDataModelWeakPtr> m_CreatedSplinesDataModels;
};

CCurveEditorDebugComponent::CCurveEditorDebugComponent(ICurveEditorView& editorView, IEditorContext& editorContext) :
    CCurveEditorViewVisibleComponentBase(editorView),
    m_EditorContext(editorContext)
{
}

void CCurveEditorDebugComponent::OnFrame(ImDrawList&, ICurveEditorController&)
{
    ImGui::Begin("Debug Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::ColorEdit4("Spline color", &m_SplineColor.Value.x);

    const auto getEditorDataModel = [this]() -> ICurveEditorDataModelSharedPtr
    {
        return std::dynamic_pointer_cast<ICurveEditorDataModel>(m_EditorContext.GetDataModel());
    };

    if (ImGui::Button("Add Spline"))
    {
        if (const auto editorDataModel = getEditorDataModel())
        {
            auto splineDataModel = editorDataModel->AddSplineDataModel(static_cast<SplineColor>(m_SplineColor));
            EDITOR_ASSERT(splineDataModel);
            m_CreatedSplinesDataModels.emplace(std::move(splineDataModel));

            const auto result = splineDataModel->AddControlPoints({
                { 0, { 1.0f, -3.0f } },
                { 1, { 2.0f, -2.0f } },
                { 2, { 3.0f, -4.0f } },
                { 3, { 4.0f, -3.0f } }
                });
            EDITOR_ASSERT(result);
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

ICurveEditorDebugComponentUniquePtr ICurveEditorDebugComponent::Create(ICurveEditorView& editorView, IEditorContext& editorContext)
{
    return std::make_unique<CCurveEditorDebugComponent>(editorView, editorContext);
}