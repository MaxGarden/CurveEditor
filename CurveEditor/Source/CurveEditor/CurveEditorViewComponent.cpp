#include "pch.h"
#include <imgui/imgui.h>
#include "CurveEditorViewComponent.h"
#include "CurveEditorDataModel.h"
#include "CurveEditorController.h"

CCurveEditorViewComponentBase::CCurveEditorViewComponentBase(const CCurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

void CCurveEditorViewComponentBase::OnFrame()
{
    EDITOR_ASSERT(ImGui::GetCurrentContext());
    const auto drawList = ImGui::GetWindowDrawList();
    if (!drawList)
        return;

    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return;

    OnFrame(*drawList, *controller);
}

void CCurveEditorViewComponentBase::OnFrame(ImDrawList&, CCurveEditorViewController&)
{
    //to override
}

const CCurveEditorView& CCurveEditorViewComponentBase::GetEditorView() const noexcept
{
    return m_EditorView;
}