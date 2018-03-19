#include "pch.h"
#include "SplineControllerFactory.h"
#include "SplineFunctionController.h"

ICurveEditorSplineControllerUniquePtr CCurveEditorSplineControllerFactory::Create(const ICurveEditorSplineDataModelSharedPtr&)
{
    return std::make_unique<CCurveEditorFunctionSplineController>();
}