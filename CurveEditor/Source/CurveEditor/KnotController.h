#pragma  once
#if !defined(__CURVE_EDITOR_KNOT_CONTROLLER__)

#include "ax/ax.h"
#include "SplineController.h"

class ICurveEditorKnotController : public ICurveEditorSplineComponentController
{
public:
    virtual ~ICurveEditorKnotController() override = default;

    virtual bool Remove() = 0;
};

class ICurveEditorKnotControllerPrivate : public ICurveEditorKnotController
{
public:
    virtual ~ICurveEditorKnotControllerPrivate() override = default;

    virtual bool SetKnotIndex(size_t knotIndex) noexcept = 0;
    virtual std::optional<size_t> GetControlPointIndex() const noexcept = 0;

    static ICurveEditorKnotControllerPrivateUniquePtr Create();
};

#endif //__CURVE_EDITOR_KNOT_CONTROLLER__