#pragma once
#if !defined(__CURVE_EDITOR_SPLINE_NAVIGATION_TOOL_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorSplineNavigationTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorSplineNavigationTool(ECurveEditorMouseButton activationMouseButton);
    virtual ~CCurveEditorSplineNavigationTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnClickUp(const CCurveEditorToolMouseButtonEvent& event) override final;

private:
    const ECurveEditorMouseButton m_ActivationMouseButton;

    ICurveEditorNavigationComponentWeakPtr m_NavigationComponent;
    ICurveEditorSplinesViewComponentWeakPtr m_SplinesViewComponent;
};

#endif //__CURVE_EDITOR_SPLINE_NAVIGATION_TOOL_H__