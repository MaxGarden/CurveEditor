#pragma  once
#if !defined(__CURVE_EDITOR_SCROLL_TOOL_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorScrollTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorScrollTool(ECurveEditorMouseButton mouseButton);
    virtual ~CCurveEditorScrollTool() override final = default;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) override final;

private:
    ECurveEditorMouseButton m_MouseButton = ECurveEditorMouseButton::Right;
    ax::pointf m_LastDragDelta;
};

#endif //__CURVE_EDITOR_SCROLL_TOOL_H__