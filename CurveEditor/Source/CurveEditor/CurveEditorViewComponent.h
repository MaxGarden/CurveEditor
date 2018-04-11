#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_COMPONENT_H__)

#include "CurveEditorView.h"

class CCurveEditorViewComponentBase : public CEditorViewBase<IEditorView, ICurveEditorController>
{
public:
    CCurveEditorViewComponentBase(const ICurveEditorView& editorView);
    virtual ~CCurveEditorViewComponentBase() override = default;

    virtual void OnFrame() override final;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorController& editorController);

    const ICurveEditorView& GetEditorView() const noexcept;

private:
    const ICurveEditorView& m_EditorView;
};

#endif //__CURVE_EDITOR_VIEW_COMPONENT_H__