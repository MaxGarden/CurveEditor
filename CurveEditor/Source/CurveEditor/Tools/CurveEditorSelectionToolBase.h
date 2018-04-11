#pragma  once
#if !defined(__CURVE_EDITOR_SELECTION_TOOL_BASE_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorSelectionToolBase final : public CCurveEditorToolBase
{
public:
    CCurveEditorSelectionToolBase(ECurveEditorMouseButton mouseButton);
    virtual ~CCurveEditorSelectionToolBase() override final = default;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) override final;

private:
    ECurveEditorMouseButton m_MouseButton = ECurveEditorMouseButton::Left;
    ax::pointf m_DragStartPosition;
};

#endif //__CURVE_EDITOR_SELECTION_TOOL_BASE_H__