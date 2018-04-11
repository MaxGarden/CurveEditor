#pragma  once
#if !defined(__CURVE_EDITOR_SCROLL_TOOL_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorScrollTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorScrollTool(ECurveEditorMouseButton mouseButton);
    virtual ~CCurveEditorScrollTool() override final = default;

    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) override final;

private:
    ECurveEditorMouseButton m_MouseButton = ECurveEditorMouseButton::Right;
};

#endif //__CURVE_EDITOR_SCROLL_TOOL_H__