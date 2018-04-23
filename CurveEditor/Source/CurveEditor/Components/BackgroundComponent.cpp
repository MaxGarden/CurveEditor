#include "pch.h"
#include "BackgroundComponent.h"
#include "CurveEditorController.h"
#include <ImGuiInterop.h>

using namespace ImGuiInterop;
using namespace ax::ImGuiInterop;

CCurveEditorBackgroundViewComponent::CCurveEditorBackgroundViewComponent(ICurveEditorView& editorView) :
    CCurveEditorViewVisibleComponentBase(editorView)
{
}

void CCurveEditorBackgroundViewComponent::OnFrame(ImDrawList& drawList, ICurveEditorController& editorController)
{
    const auto& editorView = GetEditorView();
    const auto& style = editorController.GetEditorStyle();
    const auto& editorCanvas = editorView.GetCanvas();
    const auto& windowCanvas = editorCanvas.GetWindowCanvas();

    const auto& windowPosition = to_imvec(windowCanvas.GetWindowScreenPosition());
    const auto& windowSize = to_imvec(windowCanvas.GetWindowScreenSize());

    const auto& backgroundColor = style.Colors[CurveEditorStyleColor_Background];

    drawList.AddRectFilled(windowPosition, windowPosition + windowSize, backgroundColor);
}