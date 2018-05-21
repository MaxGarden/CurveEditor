#include "pch.h"
#include "CurveEditorSplineListenerBase.h"

void CCurveEditorSplineDataModelListenerBase::OnKnotInserted(size_t)
{
    //to override
}

void CCurveEditorSplineDataModelListenerBase::OnKnotRemoved(size_t)
{
    //to override
}

void CCurveEditorSplineDataModelListenerBase::OnControlPointsPositionsChanged(const SplineControlPointsPositions&)
{
    //to override
}

void CCurveEditorSplineControllerListenerBase::OnKnotCreated(const ICurveEditorKnotControllerSharedPtr&)
{
    //to override
}

void CCurveEditorSplineControllerListenerBase::OnKnotDestroyed(const ICurveEditorKnotControllerSharedPtr&)
{
    //to override
}

void CCurveEditorSplineControllerListenerBase::OnTangentCreated(const ICurveEditorTangentControllerSharedPtr&)
{
    //to override
}

void CCurveEditorSplineControllerListenerBase::OnTangentDestroyed(const ICurveEditorTangentControllerSharedPtr&)
{
    //to override
}

void CCurveEditorSplineControllerListenerBase::OnCurveCreated(const ICurveEditorCurveControllerSharedPtr&)
{
    //to override
}

void CCurveEditorSplineControllerListenerBase::OnCurveDestroyed(const ICurveEditorCurveControllerSharedPtr&)
{
    //to override
}