#pragma  once
#if !defined(__CURVE_EDITOR_TANGENT_VIEW_COMPONENT_H__)

#include "SplineViewComponent.h"

class ICurveEditorTangentView : public ICurveEditorSplineViewComponent
{
public:
    virtual ~ICurveEditorTangentView() override = default;

    static ICurveEditorTangentViewSharedPtr Create(ICurveEditorView& editorView, size_t tangentPointIndex);
};

#endif //__CURVE_EDITOR_TANGENT_VIEW_COMPONENT_H__