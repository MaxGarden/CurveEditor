#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_COMPONENT_H__)

#include "CurveEditorView.h"

class CCurveEditorViewComponentBase : public CEditorViewBase<ICurveEditorViewComponent, ICurveEditorController>
{
public:
    CCurveEditorViewComponentBase(ICurveEditorView& editorView);
    virtual ~CCurveEditorViewComponentBase() override = default;

    virtual bool Initialize() override;
    virtual void OnFrame() override;

    virtual ICurveEditorView& GetEditorView() const noexcept override final;

private:
    ICurveEditorView& m_EditorView;
};

class CCurveEditorViewVisibleComponentBase : public CCurveEditorViewComponentBase
{
public:
    CCurveEditorViewVisibleComponentBase(ICurveEditorView& editorView);
    virtual ~CCurveEditorViewVisibleComponentBase() override = default;

    virtual void OnFrame() override final;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorController& editorController);
};

#endif //__CURVE_EDITOR_VIEW_COMPONENT_H__