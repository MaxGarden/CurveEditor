#include "pch.h"
#include "GridComponent.h"
#include <ImGuiInterop.h>
#include "CurveEditorController.h"

using namespace ImGuiInterop;

CCurveEditorGridViewComponent::CCurveEditorGridViewComponent(const CCurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView)
{
}

void CCurveEditorGridViewComponent::OnFrame(ImDrawList& drawList, CCurveEditorController& controller)
{
    static const auto graduation = ImVec2(10, 10);

    const auto& editorView = GetEditorView();
    const auto& style = controller.GetEditorStyle();
    const auto& editorCanvas = editorView.GetCanvas();
    const auto& windowCanvas = editorCanvas.GetWindowCanvas();

    const auto& offset = windowCanvas.GetClientOrigin();

    const auto gridSize = editorCanvas.CalculateScaledUnit();

    const auto& windowPosition = windowCanvas.GetWindowScreenPosition();
    const auto& windowSize = windowCanvas.GetWindowScreenSize();

    const auto& gridColor = style.Colors[EditorStyleColor_Grid];
    const auto& smallGridColor = style.Colors[EditorStyleColor_SmallGrid];

    for (float x = -gridSize.x; x < windowSize.x + gridSize.x; x += gridSize.x)
    {
        auto localX = floorf(x + fmodf(offset.x, gridSize.x));
        drawList.AddLine(ImVec2(localX, 0.0f) + windowPosition, ImVec2(localX, windowSize.y) + windowPosition, gridColor);

        auto smallGraduation = gridSize.x / graduation.x;

        for (int i = 1; i < graduation.x; ++i)
        {
            const auto graduation = floorf(localX + smallGraduation * i);
            drawList.AddLine(ImVec2(graduation, 0.0f) + windowPosition, ImVec2(graduation, windowSize.y) + windowPosition, smallGridColor);
        }
    }

    for (float y = -gridSize.y; y < windowSize.y + gridSize.y; y += gridSize.y)
    {
        auto localY = floorf(y + fmodf(offset.y, gridSize.y));
        drawList.AddLine(ImVec2(0.0f, localY) + windowPosition, ImVec2(windowSize.x, localY) + windowPosition, gridColor);

        auto smallGraduation = gridSize.y / graduation.y;

        for (int i = 1; i < graduation.y; ++i)
        {
            const auto graduation = floorf(localY + smallGraduation * i);
            drawList.AddLine(ImVec2(0.0f, graduation) + windowPosition, ImVec2(windowSize.x, graduation) + windowPosition, smallGridColor);
        }
    }
}