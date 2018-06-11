#include "pch.h"
#include "DebugComponent.h"
#include "CurveEditorViewVisibleComponentBase.h"
#include "SelectionComponent.h"
#include "CurveEditorController.h"
#include "EditorContext.h"

class CCurveEditorDebugComponent final : public CCurveEditorViewVisibleComponentBase<ICurveEditorDebugComponent>
{
public:
    CCurveEditorDebugComponent(ICurveEditorView& editorView, IEditorContext& editorContext);
    virtual ~CCurveEditorDebugComponent() override final = default;

protected:
    virtual bool Initialize() override final;
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorController& editorController) override final;

private:
    IEditorContext & m_EditorContext;
    ImColor m_SplineColor = ImColor(0.0f, 1.0f, 0.0f);

    std::stack<ICurveEditorSplineDataModelWeakPtr> m_CreatedSplinesDataModels;
    ICurveEditorSelectionViewComponentWeakPtr m_SelectionViewConponent;
};

CCurveEditorDebugComponent::CCurveEditorDebugComponent(ICurveEditorView& editorView, IEditorContext& editorContext) :
    CCurveEditorViewVisibleComponentBase(editorView),
    m_EditorContext(editorContext)
{
}

bool CCurveEditorDebugComponent::Initialize()
{
    m_SelectionViewConponent = GetViewComponent<ICurveEditorSelectionViewComponent>(GetEditorView());
    EDITOR_ASSERT(!m_SelectionViewConponent.expired());

    return !m_SelectionViewConponent.expired();
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
            const auto splineID = editorDataModel->GetFreeSplineID();
            auto splineDataModel = ICurveEditorSplineDataModel::Create(splineID, static_cast<SplineColor>(m_SplineColor), ECurveEditorSplineType::Function);

            auto addResult = editorDataModel->AddSplineDataModel(std::move(splineDataModel));
            EDITOR_ASSERT(addResult);
            m_CreatedSplinesDataModels.emplace(editorDataModel->GetSplineDataModel(splineID));

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

    if (const auto selectionViewComponent = m_SelectionViewConponent.lock())
    {
        auto selectedComponentsCount = 0u;
        selectionViewComponent->VisitSelection([&selectedComponentsCount](const auto&)
        {
            ++selectedComponentsCount;
        });

        ImGui::Text("Selected components count: %d", selectedComponentsCount);
    }

    ImGui::End();
}

ICurveEditorDebugComponentUniquePtr ICurveEditorDebugComponent::Create(ICurveEditorView& editorView, IEditorContext& editorContext)
{
    return std::make_unique<CCurveEditorDebugComponent>(editorView, editorContext);
}