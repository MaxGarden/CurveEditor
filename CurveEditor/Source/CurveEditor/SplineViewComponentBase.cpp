#include "pch.h"
#include "SplineViewComponentBase.h"
#include "CurveEditorView.h"

CCurveEditorSplineViewComponentBase::CCurveEditorSplineViewComponentBase(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

void CCurveEditorSplineViewComponentBase::OnFrame()
{
    const auto drawList = ImGui::GetWindowDrawList();
    EDITOR_ASSERT(drawList);
    if (!drawList)
        return;

    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return;

    OnFrame(*drawList, *controller);
}

void CCurveEditorSplineViewComponentBase::OnFrame(ImDrawList&, ICurveEditorSplineController&)
{
    //to override
}

ICurveEditorView& CCurveEditorSplineViewComponentBase::GetEditorView() noexcept
{
    return m_EditorView;
}

const ICurveEditorView& CCurveEditorSplineViewComponentBase::GetEditorView() const noexcept
{
    return m_EditorView;
}

const SCurveEditorStyle& CCurveEditorSplineViewComponentBase::GetEditorStyle() const noexcept
{
    static const auto null = SCurveEditorStyle{};

    if (const auto editorController = GetEditorView().GetController())
        editorController->GetEditorStyle();

    return null;;
}