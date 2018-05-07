#pragma  once
#if !defined(__CURVE_EDITOR_TANGENT_VIEW_COMPONENT_H__)

#include "SplineComponentView.h"

class ICurveEditorTangentView : public ICurveEditorSplineComponentView
{
public:
    virtual ~ICurveEditorTangentView() override = default;

    static ICurveEditorTangentViewSharedPtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_TANGENT_VIEW_COMPONENT_H__