#include "pch.h"
#include "BackgroundComponent.h"
#include <ImGuiInterop.h>

using namespace ImGuiInterop;

CCurveEditorBackgroundViewComponent::CCurveEditorBackgroundViewComponent(const CCurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView)
{
}

void CCurveEditorBackgroundViewComponent::OnFrame(ImDrawList& drawList, const CCurveEditorDataModel& dataModel)
{
    const auto& editorView = GetEditorView();
    const auto& style = dataModel.GetStyle();
    const auto& editorCanvas = editorView.GetCanvas();
    const auto& windowCanvas = editorCanvas.GetWindowCanvas();

    const auto& windowPosition = windowCanvas.GetWindowScreenPosition();
    const auto& windowSize = windowCanvas.GetWindowScreenSize();

    const auto& backgroundColor = style.Colors[EditorStyleColor_Background];

    drawList.AddRectFilled(windowPosition, windowPosition + windowSize, backgroundColor);
}