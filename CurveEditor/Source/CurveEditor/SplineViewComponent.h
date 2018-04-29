#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_COMPONENT_H__)

#include <ax/ax.h>
#include "EditorView.h"

enum class ECurveEditorSplineComponentType
{
    Knot,
    Curve,
    Tangent,
};

class ICurveEditorSplineViewComponent : public IEditorView
{
public:
    ICurveEditorSplineViewComponent() = default;
    virtual ~ICurveEditorSplineViewComponent() override = default;

    virtual bool IsColliding(const ax::pointf& position, float extraThickness = 0.0f) const noexcept = 0;
    virtual bool IsColliding(const ax::rectf& rect, bool allowIntersect = true) const noexcept = 0;

    virtual ECurveEditorSplineComponentType GetType() const noexcept = 0;

    virtual const ICurveEditorSplineControllerSharedPtr& GetController() const noexcept = 0;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_COMPONENT_H__