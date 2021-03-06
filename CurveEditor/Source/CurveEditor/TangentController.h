#pragma once
#if !defined(__CURVE_EDITOR_TANGENT_CONTROLLER__)

#include "ax/ax.h"
#include "SplineController.h"

class ICurveEditorTangentController : public ICurveEditorSplineComponentController
{
public:
    virtual ~ICurveEditorTangentController() override = default;

    virtual std::optional<ax::pointf> GetAnchorPosition() const noexcept = 0;
};

class ICurveEditorTangentControllerPrivate : public ICurveEditorTangentController
{
public:
    virtual ~ICurveEditorTangentControllerPrivate() override = default;

    virtual bool SetTangentIndex(size_t tangentIndex) noexcept = 0;
    virtual std::optional<size_t> GetControlPointIndex() const noexcept = 0;

    static ICurveEditorTangentControllerPrivateUniquePtr Create();
};

#endif //__CURVE_EDITOR_TANGENT_CONTROLLER__