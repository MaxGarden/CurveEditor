#pragma once
#if !defined(__CURVE_EDITOR_SPLINE_REMOVER_TOOL_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorSplineRemoverTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorSplineRemoverTool(ECurveEditorMouseButton removeButton = ECurveEditorMouseButton::Left);
    virtual ~CCurveEditorSplineRemoverTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnClickUp(const CCurveEditorToolMouseButtonEvent& event) override final;

private:
    const ECurveEditorMouseButton m_RemoveButton;

    ICurveEditorSplinesViewComponentWeakPtr m_SpinesViewComponent;
};

#endif //__CURVE_EDITOR_SPLINE_REMOVER_TOOL_H__