#pragma  once
#if !defined(__CURVE_EDITOR_KNOT_VIEW_COMPONENT_H__)

#include "SplineComponentView.h"

class ICurveEditorKnotView : public ICurveEditorSplineComponentView
{
public:
    virtual ~ICurveEditorKnotView() override = default;

    static ICurveEditorKnotViewSharedPtr Create(ICurveEditorView& editorView, size_t knotIndex);
};

#endif //__CURVE_EDITOR_KNOT_VIEW_COMPONENT_H__