#pragma  once
#if !defined(__CURVE_EDITOR_HOVERING_TOOL_H__)

#include "CurveEditorVisibleToolBase.h"

class CCurveEditorHoveringTool final : public CCurveEditorVisibleToolBase
{
public:
    CCurveEditorHoveringTool() = default;
    virtual ~CCurveEditorHoveringTool() override final = default;

    virtual void OnActiveEditorViewChanged(const CCurveEditorToolEvent& event) override final;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) override final;

private:
    ax::pointf m_DragBeginPosition;
    std::optional<ToolViewHandle> m_HoveringViewHandle;
    ICurveEditorSplinesViewComponentWeakPtr m_SplinesViewComponent;
};

#endif //__CURVE_EDITOR_HOVERING_TOOL_H__