#pragma  once
#if !defined(__CURVE_EDITOR_KNOT_REMOVER_TOOL_H__)

#include "CurveEditorToolBase.h"

class CCurveEditorKnotRemoverTool final : public CCurveEditorToolBase
{
public:
    CCurveEditorKnotRemoverTool(ECurveEditorMouseButton removeButton = ECurveEditorMouseButton::Left);
    virtual ~CCurveEditorKnotRemoverTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnClickUp(const CCurveEditorToolMouseButtonEvent& event) override final;

private:
    ICurveEditorKnotView * GetKnotViewAtPosition(const ax::pointf& position) const noexcept;

private:
    const ECurveEditorMouseButton m_RemoveButton;

    ICurveEditorSplinesViewComponentWeakPtr m_SpinesViewComponent;
};

#endif //__CURVE_EDITOR_KNOT_REMOVER_TOOL_H__