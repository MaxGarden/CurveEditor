#pragma  once
#if !defined(__CURVE_EDITOR_KNOT_VIEW_COMPONENT_H__)

#include "SplineViewComponent.h"

class ICurveEditorKnotView : public ICurveEditorSplineViewComponent
{
public:
    virtual ~ICurveEditorKnotView() override = default;

    static ICurveEditorKnotViewSharedPtr Create(ICurveEditorView& editorView, size_t knotIndex);
};

#endif //__CURVE_EDITOR_KNOT_VIEW_COMPONENT_H__