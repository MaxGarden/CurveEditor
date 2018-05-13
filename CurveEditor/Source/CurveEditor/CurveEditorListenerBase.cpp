#include "pch.h"
#include "CurveEditorListenerBase.h"

void CCurveEditorDataModelListenerBase::OnStyleChanged(const SCurveEditorStyle&)
{
    //to override
}

void CCurveEditorDataModelListenerBase::OnSplineCreated(const ICurveEditorSplineDataModelSharedPtr&)
{
    //to override
}

void CCurveEditorDataModelListenerBase::OnSplineDestroyed(const ICurveEditorSplineDataModelSharedPtr&)
{
    //to override
}

void CCurveEditorDataModelListenerBase::OnSelectionDataModelChanged(const ICurveEditorSelectionDataModelSharedPtr&)
{
    //to override
}

void CCurveEditorControllerListenerBase::OnSplineCreated(const ICurveEditorSplineControllerSharedPtr&)
{
    //to override
}

void CCurveEditorControllerListenerBase::OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr&)
{
    //to override
}