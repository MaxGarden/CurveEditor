#pragma once
#if !defined(__CURVE_EDITOR_SELECTION_TOOL_BASE_H__)

#include "CurveEditorVisibleToolBase.h"

class CCurveEditorSelectionToolBase : public CCurveEditorVisibleToolBase
{
public:
    CCurveEditorSelectionToolBase(ECurveEditorMouseButton activationMouseButton);
    virtual ~CCurveEditorSelectionToolBase() override = default;

    virtual void OnDragBegin(const CCurveEditorToolMouseButtonEvent& event) override final;
    virtual void OnDragUpdate(const CCurveEditorToolMouseDragEvent& event) override final;
    virtual void OnDragEnd(const CCurveEditorToolMouseButtonEvent& event) override final;

    const std::optional<ax::rectf>& GetSelectionRect() const noexcept;

protected:
    virtual bool AcceptSelection(const CCurveEditorToolMouseButtonEvent& event);

    virtual void OnSelectionBegin(ICurveEditorView& editorView);
    virtual void OnSelectionUpdate(ICurveEditorView& editorView, const ax::rectf& selectionRect);
    virtual void OnSelectionEnd(ICurveEditorView& editorView);

    bool CheckActivationButton(const CCurveEditorToolMouseButtonEvent& event) const noexcept;

private:
    const ECurveEditorMouseButton m_ActivationMouseButton = ECurveEditorMouseButton::Left;

    bool m_SelectionAccepted = false;
    ax::pointf m_DragStartPosition;
    std::optional<ax::rectf> m_SelectionRect;
    std::optional<ToolViewHandle> m_SelectionViewHandle;
};

#endif //__CURVE_EDITOR_SELECTION_TOOL_BASE_H__