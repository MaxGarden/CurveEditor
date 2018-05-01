#pragma  once
#if !defined(__CURVE_EDITOR_CURVE_VIEW_COMPONENT_H__)

#include "SplineViewComponent.h"

class ICurveEditorCurveView : public ICurveEditorSplineViewComponent
{
public:
    virtual ~ICurveEditorCurveView() override = default;

    static ICurveEditorCurveViewSharedPtr Create(ICurveEditorView& editorView, size_t curveIndex);
};

#endif //__CURVE_EDITOR_CURVE_VIEW_COMPONENT_H__