#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_H__)

#include "EditorView.h"

class CCurveEditorSplineView final : public IEditorView
{
public:
    CCurveEditorSplineView(CCurveEditorView& editorView);
    virtual ~CCurveEditorSplineView() override final = default;

    virtual void OnFrame() override final;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept override final;

private:
    CCurveEditorView& m_EditorView;

    CCurveEditorFunctionSplineControllerSharedPtr m_Controller;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_H__