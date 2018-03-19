#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_H__)

#include "EditorView.h"

class ICurveEditorSplineView : public IEditorView
{
public:
    virtual ~ICurveEditorSplineView() override = default;
};

class ICurveEditorSplineViewFactory
{
public:
    virtual ~ICurveEditorSplineViewFactory() = default;

    virtual ICurveEditorSplineViewUniquePtr Create(CCurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
};

class CCurveEditorSplineView final : public ICurveEditorSplineView
{
public:
    CCurveEditorSplineView(CCurveEditorView& editorView);
    virtual ~CCurveEditorSplineView() override final = default;

    virtual void OnFrame() override final;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept override final;

private:
    CCurveEditorView& m_EditorView;

    ICurveEditorSplineControllerSharedPtr m_Controller;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_H__