#include "pch.h"
#include <imgui/imgui.h>
#include "CurveEditorViewComponent.h"
#include "CurveEditorDataModel.h"
#include "CurveEditorController.h"

CCurveEditorViewComponentBase::CCurveEditorViewComponentBase(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{

}

bool CCurveEditorViewComponentBase::Initialize()
{
    //to override
    return true;
}

void CCurveEditorViewComponentBase::OnFrame()
{
    //to override
}

ICurveEditorView& CCurveEditorViewComponentBase::GetEditorView() const noexcept
{
    return m_EditorView;
}


CCurveEditorViewVisibleComponentBase::CCurveEditorViewVisibleComponentBase(ICurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView)
{
}

void CCurveEditorViewVisibleComponentBase::OnFrame()
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

void CCurveEditorViewVisibleComponentBase::OnFrame(ImDrawList&, ICurveEditorController&)
{
    //to override
}