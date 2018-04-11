#include "pch.h"
#include "CurveEditorZoomTool.h"
#include "CurveEditorToolEvent.h"
#include "CurveEditorView.h"

void CCurveEditorZoomTool::OnWheel(const CCurveEditorToolMouseWheelEvent& event)
{
    auto& editorView = event.GetEditorView();
    auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto& mousePosition = event.GetMousePosition();

    const auto screenPosition = windowCanvas.ToScreen(mousePosition);
    const auto previousCanvasPosition = windowCanvas.FromScreen(screenPosition);

    ApplyZoom(static_cast<int>(event.GetWheelValue()), editorView);

    const auto canvasPosition = windowCanvas.FromScreen(screenPosition);
    const auto& zoom = windowCanvas.GetZoom();
    const auto offset = ax::pointf{ (canvasPosition.x - previousCanvasPosition.x) * zoom.x, (canvasPosition.y - previousCanvasPosition.y) * zoom.y };

    windowCanvas.SetClientOrigin(windowCanvas.GetClientOrigin() + offset);
}

void CCurveEditorZoomTool::ApplyZoom(int steps, ICurveEditorView& editorView)
{
    const auto& editorController = editorView.GetController();
    EDITOR_ASSERT(editorController);
    if (!editorController)
        return;

    const auto& editorStyle = editorController->GetEditorStyle();

    auto applyAxisZoom = [steps, &editorStyle](float& level, float& zoom)
    {
        level += steps;
        level = std::max(editorStyle.MinimumZoomLevel, std::min(level, editorStyle.MaximumZoomLevel));

        if (level > 0)
            zoom = pow(editorStyle.ZoomInMultipler, level);
        else
            zoom = pow(editorStyle.ZoomOutMultipler, abs(level));
    };

    auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    auto zoom = windowCanvas.GetZoom();

    applyAxisZoom(m_ZoomLevel.x, zoom.x);
    applyAxisZoom(m_ZoomLevel.y, zoom.y);

    windowCanvas.SetZoom(zoom);
}