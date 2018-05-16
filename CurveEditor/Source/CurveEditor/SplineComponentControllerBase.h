#pragma  once
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
    bool SetControlPointPosition(size_t controlPointIndex, const ax::pointf& position);
    std::optional<ax::pointf> GetControlPointPosition(size_t controlPointIndex) const noexcept;

    bool MoveControlPointPosition(size_t controlPointIndex, const ax::pointf& delta);

    const std::vector<ax::pointf>& GetControlPoints() const noexcept;

    using Super = CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>;
};

#include "SplineComponentControllerBase.inl"

#endif //__CURVE_EDITOR_SPLINE_COMPONENT_CONTROLLER_BASE_H__