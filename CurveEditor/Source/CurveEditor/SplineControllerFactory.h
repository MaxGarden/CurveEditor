#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_CONTROLLER_FACTORY_H__)

#include "SplineController.h"

class CCurveEditorSplineControllerFactory final : public ICurveEditorSplineControllerFactory
{
public:
    CCurveEditorSplineControllerFactory() = default;
    virtual ~CCurveEditorSplineControllerFactory() override final = default;

    virtual ICurveEditorSplineControllerUniquePtr Create(const ICurveEditorSplineDataModelSharedPtr& dataModel) override final;
};

#endif //__CURVE_EDITOR_SPLINE_CONTROLLER_FACTORY_H__