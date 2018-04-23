#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_H__)

#include "SplineController.h"
#include "EditorViewBase.h"
#include "Style.h"

class ICurveEditorSplineView : public IEditorView
{
public:
    virtual ~ICurveEditorSplineView() override = default;

    virtual void VisitSplineComponents(const VisitorType<ICurveEditorSplineViewComponent>& visitor) const noexcept = 0;
};

class ICurveEditorSplineViewFactory
{
public:
    virtual ~ICurveEditorSplineViewFactory() = default;

    virtual ICurveEditorSplineViewUniquePtr Create(ICurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_H__