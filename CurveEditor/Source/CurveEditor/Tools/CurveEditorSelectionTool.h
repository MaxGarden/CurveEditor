#pragma  once
#if !defined(__CURVE_EDITOR_SELECTION_TOOL_H__)

#include "CurveEditorSelectionToolBase.h"

class CCurveEditorSelectionTool final : public CCurveEditorSelectionToolBase
{
public:
    CCurveEditorSelectionTool(ECurveEditorMouseButton activationMouseButton, ECurveEditorModifier togglingModifier);
    virtual ~CCurveEditorSelectionTool() override final = default;

    virtual void OnAcquired(const CCurveEditorToolEvent& event) override final;
    virtual void OnReleased(const CCurveEditorToolEvent& event) override final;

    virtual void OnClick(const CCurveEditorToolMouseButtonEvent& event) override final;

    virtual void OnModifierActivated(const CCurveEditorToolModifierEvent& event) override final;
    virtual void OnModifierDeactivated(const CCurveEditorToolModifierEvent& event) override final;

protected:
    virtual void OnSelectionUpdate(ICurveEditorView& editorView, const ax::rectf& selectionRect) override final;
    virtual void OnSelectionEnd(ICurveEditorView& editorView) override final;

private:
    const ECurveEditorModifier m_TogglingModifier;

    bool m_TogglingMode = false;

    ICurveEditorSplinesViewComponentWeakPtr m_SplinesViewComponent;
    ICurveEditorSelectionViewComponentWeakPtr m_SelectionViewComponent;
    std::set<const ICurveEditorSplineComponentView*> m_LastSelectedSplineComponents;
};

#endif //__CURVE_EDITOR_SELECTION_TOOL_H__