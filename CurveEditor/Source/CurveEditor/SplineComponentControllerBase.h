#pragma once
#if !defined(__CURVE_EDITOR_SPLINE_COMPONENT_CONTROLLER_BASE_H__)

#include "SplineController.h"
#include "EditorControllerBase.h"

template<typename SuperClass, ECurveEditorSplineComponentType ComponentType>
class CCurveEditorSplineComponentControllerBase : public CEditorControllerBase<SuperClass, ICurveEditorSplineDataModel, IEditorListener>
{
public:
    CCurveEditorSplineComponentControllerBase() = default;
    virtual ~CCurveEditorSplineComponentControllerBase() override = default;

    virtual const SplineID& GetSplineID() const noexcept override final;
    virtual ECurveEditorSplineComponentType GetType() const noexcept override final;

protected:
    bool SetControlPoints(const SplineControlPointsPositions& positions);
    std::optional<ax::pointf> GetControlPoint(size_t controlPointIndex) const noexcept;

    bool MoveControlPoints(SplineControlPointsPositions deltaPositions);

    const std::vector<ax::pointf>& GetControlPoints() const noexcept;

    using Super = CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>;
};

#include "SplineComponentControllerBase.inl"

#endif //__CURVE_EDITOR_SPLINE_COMPONENT_CONTROLLER_BASE_H__