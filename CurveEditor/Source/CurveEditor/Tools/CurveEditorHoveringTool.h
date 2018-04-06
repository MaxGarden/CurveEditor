#pragma  once
#if !defined(__CURVE_EDITOR_HOVERING_TOOL_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorHoveringTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorHoveringTool() = default;
    virtual ~CCurveEditorHoveringTool() override final = default;

    virtual void OnMouseMove(const CCurveEditorToolMouseEvent& event) override final;
};

#endif //__CURVE_EDITOR_HOVERING_TOOL_H__