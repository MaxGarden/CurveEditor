#include "pch.h"
#include "CurveEditorViewComponent.h"
#include <imgui/imgui.h>

CCurveEditorViewComponentBase::CCurveEditorViewComponentBase(const CCurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

void CCurveEditorViewComponentBase::OnFrame()
{
    assert(ImGui::GetCurrentContext());
    const auto drawList = ImGui::GetWindowDrawList();
    if (!drawList)
        return;

    const auto& dataModel = GetEditorView().GetDataModel();
    if (!dataModel)
        return;

    OnFrame(*drawList, *dataModel);
}

void CCurveEditorViewComponentBase::OnFrame(ImDrawList&, const CCurveEditorDataModel&)
{
    //to override
}

const CCurveEditorView& CCurveEditorViewComponentBase::GetEditorView() const noexcept
{
    return m_EditorView;
}