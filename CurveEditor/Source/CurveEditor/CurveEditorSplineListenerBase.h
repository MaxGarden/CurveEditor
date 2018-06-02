#pragma once
#if !defined(__CURVE_EDITOR_SPLINE_LISTENER_BASE_H__)

#include "SplineController.h"

class CCurveEditorSplineDataModelListenerBase : public ICurveEditorSplineDataModelListener
{
public:
    CCurveEditorSplineDataModelListenerBase() = default;
    virtual ~CCurveEditorSplineDataModelListenerBase() override = default;

    virtual void OnControlPointsAdded(const SplineControlPointsPositions& positions) override;
    virtual void OnControlPointsRemoved(const SplineControlPointsIndexes& indexes) override;

    virtual void OnControlPointsModified(const SplineControlPointsPositions& positions) override;
};

class CCurveEditorSplineControllerListenerBase : public ICurveEditorSplineControllerListener
{
public:
    CCurveEditorSplineControllerListenerBase() = default;
    virtual ~CCurveEditorSplineControllerListenerBase() override = default;

    virtual void OnKnotCreated(const ICurveEditorKnotControllerSharedPtr& knotController) override;
    virtual void OnKnotDestroyed(const ICurveEditorKnotControllerSharedPtr& knotController) override;

    virtual void OnTangentCreated(const ICurveEditorTangentControllerSharedPtr& tangentController) override;
    virtual void OnTangentDestroyed(const ICurveEditorTangentControllerSharedPtr& tangentController) override;

    virtual void OnCurveCreated(const ICurveEditorCurveControllerSharedPtr& curveController) override;
    virtual void OnCurveDestroyed(const ICurveEditorCurveControllerSharedPtr& curveController) override;
};

#endif //__CURVE_EDITOR_SPLINE_LISTENER_BASE_H__