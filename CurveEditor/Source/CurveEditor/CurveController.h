#pragma  once
#if !defined(__CURVE_EDITOR_CURVE_CONTROLLER__)

#include "ax/ax.h"
#include "SplineController.h"

class ICurveEditorCurveController : public IEditorController
{
public:
    virtual ~ICurveEditorCurveController() override = default;

    virtual bool VisitCurvePoints(const ConstVisitorType<ax::pointf>& visitor) const noexcept = 0;
    virtual const SplineColor& GetColor() const noexcept = 0;
};

class ICurveEditorCurveControllerPrivate : public ICurveEditorCurveController
{
public:
    virtual ~ICurveEditorCurveControllerPrivate() override = default;

    virtual bool SetCurveIndex(size_t curveIndex) noexcept = 0;

    static ICurveEditorCurveControllerPrivateUniquePtr Create();
};

#endif //__CURVE_EDITOR_SPLINE_CONTROLLER__