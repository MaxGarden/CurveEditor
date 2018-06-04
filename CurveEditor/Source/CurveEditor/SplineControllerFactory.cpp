#include "pch.h"
#include "SplineControllerFactory.h"
#include "SplineFunctionController.h"
#include "SplinePathController.h"

ICurveEditorSplineControllerUniquePtr CCurveEditorSplineControllerFactory::Create(const ICurveEditorSplineDataModelSharedPtr& splineDataModel)
{
    EDITOR_ASSERT(splineDataModel);
    if (!splineDataModel)
        return nullptr;

    switch (splineDataModel->GetType())
    {
    case ECurveEditorSplineType::Function:
        return ICurveEditorFunctionSplineController::Create();
    case ECurveEditorSplineType::Path:
        return ICurveEditorPathSplineController::Create();
    default:
        return nullptr;
    };
}