#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_COMPONENT_H__)

#include "CurveEditorView.h"

class CCurveEditorViewComponentBase : public CEditorViewBase<IEditorView, ICurveEditorController>
{
public:
    CCurveEditorViewComponentBase(const CCurveEditorView& editorView);
    virtual ~CCurveEditorViewComponentBase() override = default;

    virtual void OnFrame() override final;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorController& editorController);

    const CCurveEditorView& GetEditorView() const noexcept;

private:
    const CCurveEditorView& m_EditorView;
};

#endif //__CURVE_EDITOR_VIEW_COMPONENT_H__