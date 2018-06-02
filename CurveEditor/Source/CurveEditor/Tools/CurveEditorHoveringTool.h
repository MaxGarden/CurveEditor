#pragma once
#if !defined(__CURVE_EDITOR_HOVERING_TOOL_H__)

#include "CurveEditorVisibleToolBase.h"
#include "SplineController.h"

class CCurveEditorHoveringTool final : public CCurveEditorVisibleToolBase
{
public:
    CCurveEditorHoveringTool(std::set<ECurveEditorSplineComponentType>&& hoveredSplineComponentsTypes, bool curveAsSpline = false);
    virtual ~CCurveEditorHoveringTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnMouseMove(const CCurveEditorToolMouseEvent& event) override final;

private:
    void UpdateHoveringView(ICurveEditorSplineComponentView* splineViewComponent, ICurveEditorView& editorView);
    std::vector<IEditorRenderableUniquePtr> CreateBorderRenderables(ICurveEditorSplineComponentView& splineComponentView) const;

private:
    const bool m_CurveAsSpline;
    const std::set<ECurveEditorSplineComponentType> m_HoveredSplineComponentsTypes;

    ax::pointf m_DragBeginPosition;
    std::optional<ToolViewHandle> m_HoveringViewHandle;
    ICurveEditorSplinesViewComponentWeakPtr m_SplinesViewComponent;
    ICurveEditorSplineComponentView* m_LastHoveredSplineViewComponent = nullptr;
};

#endif //__CURVE_EDITOR_HOVERING_TOOL_H__