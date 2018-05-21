#pragma  once
#if !defined(__CURVE_EDITOR_MOVING_TOOL_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorMovingTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorMovingTool(ECurveEditorMouseButton activationMouseButton = ECurveEditorMouseButton::Left);
    virtual ~CCurveEditorMovingTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) override final;
    virtual void OnDragEnd(const CCurveEditorToolMouseButtonEvent& event) override final;

private:
    const ECurveEditorMouseButton m_ActivationMouseButton;

    ICurveEditorSplinesViewComponentWeakPtr m_SplinesViewComponent;
    ICurveEditorSelectionViewComponentWeakPtr m_SelectionViewComponent;

    std::map<ICurveEditorSplineComponentView*, const ax::pointf> m_DraggingSplineComponentsViews;
};

#endif //__CURVE_EDITOR_MOVING_TOOL_H__