#include "pch.h"
#include "CurveEditorSplineListenerBase.h"

void CCurveEditorSplineDataModelListenerBase::OnControlPointsAdded(const SplineControlPointsPositions&)
{
    //to override
}

void CCurveEditorSplineDataModelListenerBase::OnControlPointsRemoved(const SplineControlPointsIndexes&)
{
    //to override
}

void CCurveEditorSplineDataModelListenerBase::OnControlPointsModified(const SplineControlPointsPositions&)
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