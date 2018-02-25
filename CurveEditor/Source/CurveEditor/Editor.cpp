#include "Editor.h"
#include <imgui.h>

class CCurveEditorView final : public IEditorView
{
public:
    CCurveEditorView() = default;
    virtual ~CCurveEditorView() override final = default;

    virtual void OnFrame() override final;

private:
    void OnFrame(ImDrawList& drawList);
};

void CCurveEditorView::OnFrame()
{
    //ImGui::BeginChildFrame(1, ImVec2(300, 300));

    if (const auto drawList = ImGui::GetWindowDrawList())
        OnFrame(*drawList);

    //ImGui::Button("Test");

    //ImGui::EndChildFrame();
}

void CCurveEditorView::OnFrame(ImDrawList& drawList)
{
    drawList.AddCircle(ImVec2(50, 100), 100.0f, ImColor(1.0, 0.0f, 0.0f), 12, 10);
}

std::unique_ptr<IEditorView> CCurveEditorFactory::Create()
{
    return std::make_unique<CCurveEditorView>();
}