#include "Editor.h"
#include <ax/ax.h>

CEditorCanvas& CCurveEditorView::GetCanvas() noexcept
{
    return m_Canvas;
}

const CEditorCanvas& CCurveEditorView::GetCanvas() const noexcept
{
    return m_Canvas;
}

const SEditorStyle& CCurveEditorView::GetStyle() const noexcept
{
    return m_EditorStyle;
}

CCurveEditorViewComponent::CCurveEditorViewComponent(const CCurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

void CCurveEditorViewComponent::OnFrame()
{
    assert(ImGui::GetCurrentContext());
    if (const auto drawList = ImGui::GetWindowDrawList())
        OnFrame(*drawList);
}

void CCurveEditorViewComponent::OnFrame(ImDrawList&)
{
    //to override
}

const CCurveEditorView& CCurveEditorViewComponent::GetEditorView() const noexcept
{
    return m_EditorView;
}