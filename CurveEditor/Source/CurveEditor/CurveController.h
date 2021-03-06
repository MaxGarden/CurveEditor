#pragma once
#if !defined(__CURVE_EDITOR_CURVE_CONTROLLER__)

#include "ax/ax.h"
#include "SplineController.h"

class ICurveEditorCurveController : public ICurveEditorSplineComponentController
{
public:
    virtual ~ICurveEditorCurveController() override = default;

    virtual bool InsertKnot(float tPosition) = 0;

    virtual bool VisitCurveControlPoints(const ConstVisitorType<ax::pointf>& visitor) const = 0;
    virtual const SplineColor& GetColor() const noexcept = 0;
};

class ICurveEditorCurveControllerPrivate : public ICurveEditorCurveController
{
public:
    virtual ~ICurveEditorCurveControllerPrivate() override = default;

    virtual bool SetCurveIndex(size_t curveIndex) noexcept = 0;
    virtual std::optional<size_t> GetControlPointIndex() const noexcept = 0;

    static ICurveEditorCurveControllerPrivateUniquePtr Create();
};

#endif //__CURVE_EDITOR_SPLINE_CONTROLLER__