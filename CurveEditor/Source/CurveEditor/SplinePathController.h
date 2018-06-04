#pragma once
#if !defined(__CURVE_EDITOR_PATH_SPLINE_CONTROLLER__)

#include "SplineController.h"

class ICurveEditorPathSplineController : public ICurveEditorSplineController
{
public:
    virtual ~ICurveEditorPathSplineController() override = default;

    static ICurveEditorPathSplineControllerUniquePtr Create();
};

#endif //__CURVE_EDITOR_PATH_SPLINE_CONTROLLER__