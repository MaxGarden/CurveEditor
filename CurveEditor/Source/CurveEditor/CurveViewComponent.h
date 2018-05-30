#pragma  once
#if !defined(__CURVE_EDITOR_CURVE_VIEW_COMPONENT_H__)

#include "SplineComponentView.h"

class ICurveEditorCurveView : public ICurveEditorSplineComponentView
{
public:
    virtual ~ICurveEditorCurveView() override = default;

    virtual bool InsertKnot(float position) = 0;
    virtual std::optional<ax::pointf> GetClosestPosition(const ax::pointf& position) const noexcept = 0;

    static ICurveEditorCurveViewSharedPtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_CURVE_VIEW_COMPONENT_H__