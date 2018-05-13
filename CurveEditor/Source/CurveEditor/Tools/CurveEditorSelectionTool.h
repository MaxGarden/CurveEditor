#pragma  once
#if !defined(__CURVE_EDITOR_SELECTION_TOOL_H__)

#include "SplineComponentView.h"
#include "CurveEditorSelectionToolBase.h"

class CCurveEditorSelectionTool final : public CCurveEditorSelectionToolBase
{
public:
    CCurveEditorSelectionTool(ECurveEditorMouseButton activationMouseButton, ECurveEditorModifier togglingModifier);
    virtual ~CCurveEditorSelectionTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnClickUp(const CCurveEditorToolMouseButtonEvent& event) override final;

    virtual void OnModifierActivated(const CCurveEditorToolModifierEvent& event) override final;
    virtual void OnModifierDeactivated(const CCurveEditorToolModifierEvent& event) override final;

protected:
    virtual bool AcceptSelection(const CCurveEditorToolMouseButtonEvent& event) override final;

    virtual void OnSelectionBegin(ICurveEditorView& editorView) override final;
    virtual void OnSelectionUpdate(ICurveEditorView& editorView, const ax::rectf& selectionRect) override final;
    virtual void OnSelectionEnd(ICurveEditorView& editorView) override final;

private:
    const ECurveEditorModifier m_TogglingModifier;

    bool m_TogglingMode = false;

    bool m_TogglingSelection = false;

    ECurveEditorSplineComponentType m_SelectionType = ECurveEditorSplineComponentType::Knot;
    ICurveEditorSplinesViewComponentWeakPtr m_SplinesViewComponent;
    ICurveEditorSelectionViewComponentWeakPtr m_SelectionViewComponent;
    std::set<const ICurveEditorSplineComponentView*> m_LastSelectedSplineComponents;
};

#endif //__CURVE_EDITOR_SELECTION_TOOL_H__