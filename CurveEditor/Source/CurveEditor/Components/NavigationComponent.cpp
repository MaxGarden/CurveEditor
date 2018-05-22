#include "pch.h"
#include "NavigationComponent.h"
#include "CurveEditorViewComponentBase.h"

class CCurveEditorNavigationComponent final : public CCurveEditorViewComponentBase<ICurveEditorNavigationComponent>
{
public:
    CCurveEditorNavigationComponent(ICurveEditorView& editorView);
    virtual ~CCurveEditorNavigationComponent() override final = default;

    virtual void ApplyZoom(const ax::pointf& focusPoint, int steps) override final;

private:
    ax::pointf m_ZoomLevel;
};

CCurveEditorNavigationComponent::CCurveEditorNavigationComponent(ICurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView)
{
}

void CCurveEditorNavigationComponent::ApplyZoom(const ax::pointf& focusPoint, int steps)
{
    auto& editorView = GetEditorView();
    const auto& editorStyle = editorView.GetEditorStyle();

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

    const auto screenPosition = windowCanvas.ToScreen(focusPoint);
    const auto previousCanvasPosition = windowCanvas.FromScreen(screenPosition);

    windowCanvas.SetZoom(zoom);

    const auto canvasPosition = windowCanvas.FromScreen(screenPosition);
    const auto offset = ax::pointf{ (canvasPosition.x - previousCanvasPosition.x) * zoom.x, (canvasPosition.y - previousCanvasPosition.y) * zoom.y };

    windowCanvas.SetClientOrigin(windowCanvas.GetClientOrigin() + offset);
}

ICurveEditorNavigationComponentUniquePtr ICurveEditorNavigationComponent::Create(ICurveEditorView& editorView)
{
    return std::make_unique<CCurveEditorNavigationComponent>(editorView);
}