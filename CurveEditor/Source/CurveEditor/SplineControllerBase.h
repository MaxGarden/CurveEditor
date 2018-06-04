#pragma once
#if !defined(__CURVE_EDITOR_SPLINE_CONTROLLER_BASE__)

#include "SplineDataModel.h"
#include "SplineController.h"
#include "EditorControllerBase.h"
#include "SplineFunctionController.h"
#include "CurveEditorController.h"
#include "CurveEditorSplineListenerBase.h"
#include "KnotController.h"
#include "TangentController.h"
#include "CurveController.h"

template<typename SuperClass>
class CCurveEditorSplineControllerBase : public CEditorControllerBase<SuperClass, ICurveEditorSplineDataModel, ICurveEditorSplineControllerListener>
{
public:
    CCurveEditorSplineControllerBase() = default;
    virtual ~CCurveEditorSplineControllerBase() override = default;

    virtual bool BeginEditing() override;
    virtual bool EndEditing() override;

    virtual bool SaveState() override final;
    virtual bool RestoreState() override final;
    virtual void ResetSavedState() noexcept override final;

    virtual const SplineID& GetID() const noexcept override final;
    virtual const SplineColor& GetColor() const noexcept override final;

    virtual void VisitKnotsControllers(const ConstVisitorType<ICurveEditorKnotControllerSharedPtr>& visitor) const override final;
    virtual void VisitTangentsControllers(const ConstVisitorType<ICurveEditorTangentControllerSharedPtr>& visitor) const override final;
    virtual void VisitCurvesControllers(const ConstVisitorType<ICurveEditorCurveControllerSharedPtr>& visitor) const override final;

    virtual ICurveEditorSplineComponentControllerSharedPtr GetSplineComponentController(ECurveEditorSplineComponentType type, size_t index) const noexcept override final;

    void OnControlPointsAdded(const SplineControlPointsPositions& positions);
    void OnControlPointsRemoved(const SplineControlPointsIndexes& indexes);

protected:
    virtual void OnDataModelChanged() override final;

    template<typename Container, typename GetMethod>
    bool SortComponentsByPosition(Container& container, GetMethod getPositionMethod) const;

    template<typename Container>
    bool SortComponentsByIndex(Container& container) const;

    template<typename Container>
    void UpdateComponents(Container& container) const;

    size_t CalculateKnotsCount() const noexcept;
    size_t CalculateTangentsCount() const noexcept;
    size_t CalculateCurvesCount() const noexcept;

    const std::vector<ax::pointf>& GetControlPoints() const noexcept;

    using KnotsControllers = std::vector<ICurveEditorKnotControllerPrivateSharedPtr>;
    using TangentsControllers = std::vector<ICurveEditorTangentControllerPrivateSharedPtr>;
    using CurvesControllers = std::vector<ICurveEditorCurveControllerPrivateSharedPtr>;

    KnotsControllers& GetKnotsControllers() noexcept;
    TangentsControllers& GetTangentsControllers() noexcept;
    CurvesControllers& GetCurvesControllers() noexcept;

private:
    void CreateComponentsControllers();
    void DestroyComponentsControllers();

    ICurveEditorKnotControllerPrivateSharedPtr AddKnotController(size_t knotIndex);
    bool RemoveKnotController(const ICurveEditorKnotControllerPrivateSharedPtr& knotController);

    ICurveEditorTangentControllerPrivateSharedPtr AddTangentController(size_t tangentIndex);
    bool RemoveTangentController(const ICurveEditorTangentControllerPrivateSharedPtr& tangentController);

    ICurveEditorCurveControllerPrivateSharedPtr AddCurveController(size_t curveIndex);
    bool RemoveCurveController(const ICurveEditorCurveControllerPrivateSharedPtr& curveController);

private:
    KnotsControllers m_KnotsControllers;
    TangentsControllers m_TangentsControllers;
    CurvesControllers m_CurvesControllers;

    SplineControlPointsPositions m_SavedControlPointsPositions;
    KnotsControllers m_SavedKnotsControllers;
    TangentsControllers m_SavedTangentsControllers;
};

#include "SplineControllerBase.inl"

#endif //__CURVE_EDITOR_SPLINE_CONTROLLER_BASE__