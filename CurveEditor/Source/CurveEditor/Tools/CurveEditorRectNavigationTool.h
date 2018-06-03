#pragma once
#if !defined(__CURVE_EDITOR_RECT_NAVIGATION_TOOL_H__)

#include "CurveEditorSelectionToolBase.h"

class CCurveEditorRectNavigationTool final : public CCurveEditorSelectionToolBase
{
public:
    CCurveEditorRectNavigationTool(ECurveEditorMouseButton activationMouseButton);
    virtual ~CCurveEditorRectNavigationTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

protected:
    virtual void OnSelectionEnd(ICurveEditorView& editorView) override final;

private:
    ICurveEditorNavigationComponentWeakPtr m_NavigationComponent;
};

#endif //__CURVE_EDITOR_RECT_NAVIGATION_TOOL_H__