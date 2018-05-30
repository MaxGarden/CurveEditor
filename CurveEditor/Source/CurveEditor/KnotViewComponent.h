#pragma  once
#if !defined(__CURVE_EDITOR_KNOT_VIEW_COMPONENT_H__)

#include "SplineComponentView.h"

class ICurveEditorKnotView : public ICurveEditorSplineComponentView
{
public:
    virtual ~ICurveEditorKnotView() override = default;

    virtual bool Remove() = 0;

    static ICurveEditorKnotViewSharedPtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_KNOT_VIEW_COMPONENT_H__