#pragma once
#if !defined(__CURVE_EDITOR_FUNCTION_SPLINE_CONTROLLER__)

#include "SplineDataModel.h"
#include "SplineController.h"
#include "EditorControllerBase.h"

class ICurveEditorFunctionSplineController : public ICurveEditorSplineController
{
public:
    virtual ~ICurveEditorFunctionSplineController() override = default;

    static ICurveEditorFunctionSplineControllerUniquePtr Create();
};

#endif //__CURVE_EDITOR_FUNCTION_SPLINE_CONTROLLER__