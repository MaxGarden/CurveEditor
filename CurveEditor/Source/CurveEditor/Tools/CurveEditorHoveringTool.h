#pragma  once
#if !defined(__CURVE_EDITOR_HOVERING_TOOL_H__)

#include "CurveEditorVisibleToolBase.h"

class CCurveEditorHoveringTool final : public CCurveEditorVisibleToolBase
{
public:
    CCurveEditorHoveringTool() = default;
    virtual ~CCurveEditorHoveringTool() override final = default;

    virtual void OnActiveEditorViewChanged(const CCurveEditorToolEvent& event) override final;

    virtual void OnMouseMove(const CCurveEditorToolMouseEvent& event) override final;

private:
    std::optional<ToolViewHandle> m_HoveringViewHandle;
    CCurveEditorSplinesViewComponentWeakPtr m_SplinesViewComponent;
};

#endif //__CURVE_EDITOR_HOVERING_TOOL_H__