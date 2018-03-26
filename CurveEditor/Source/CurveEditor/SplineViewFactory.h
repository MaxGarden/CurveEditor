#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_FACTORY_H__)

#include "SplineView.h"

class CCurveEditorSplineViewFactory final : public ICurveEditorSplineViewFactory
{
public:
    CCurveEditorSplineViewFactory() = default;
    virtual ~CCurveEditorSplineViewFactory() override final = default;

    virtual ICurveEditorSplineViewUniquePtr Create(CCurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr& splineController) override final;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_FACTORY_H__