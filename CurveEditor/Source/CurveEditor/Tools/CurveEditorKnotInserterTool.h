#pragma  once
#if !defined(__CURVE_EDITOR_KNOT_INSERTER_TOOL_H__)

#include "CurveEditorVisibleToolBase.h"

class CCurveEditorKnotInserterTool final : public CCurveEditorVisibleToolBase
{
public:
    CCurveEditorKnotInserterTool(ECurveEditorMouseButton insertButton = ECurveEditorMouseButton::Left);
    virtual ~CCurveEditorKnotInserterTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnClickUp(const CCurveEditorToolMouseButtonEvent& event) override final;

    virtual void OnMouseMove(const CCurveEditorToolMouseEvent& event) override final;

    std::optional<ax::pointf> GetCandidateInsertPosition() const noexcept;

private:
    ICurveEditorCurveView* GetCurveViewAtPosition(const ax::pointf& position) const noexcept;
    void EnsureToolView(ICurveEditorView& editorView);
    void ResetToolView();

private:
    const ECurveEditorMouseButton m_InsertButton;

    std::optional<ToolViewHandle> m_ToolViewHandle;
    std::optional<ax::pointf> m_CandidateInsertPosition;

    ICurveEditorSplinesViewComponentWeakPtr m_SpinesViewComponent;
};

#endif //__CURVE_EDITOR_KNOT_INSERTER_TOOL_H__