#pragma once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_FACTORY_H__)

#include "SplineView.h"
#include "EditorViewBase.h"
#include "KnotViewComponent.h"
#include "CurveViewComponent.h"
#include "TangentViewComponent.h"

class CCurveEditorSplineViewFactory final : public ICurveEditorSplineViewFactory
{
public:
    CCurveEditorSplineViewFactory() = default;
    virtual ~CCurveEditorSplineViewFactory() override final = default;

    virtual ICurveEditorSplineViewUniquePtr Create(ICurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr& splineController) override final;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_FACTORY_H__