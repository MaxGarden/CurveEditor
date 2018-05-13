#include "pch.h"
#include "CurveEditorSelectionListenerBase.h"

void CCurveEditorSelectionDataModelListenerBase::OnSelectionModeChanged(ECurveEditorSplineComponentType)
{
    //to override
}

void CCurveEditorSelectionDataModelListenerBase::OnAddedToSelection(const CurveEditorDataModelSelection&)
{
    //to override
}

void CCurveEditorSelectionDataModelListenerBase::OnRemovedFromSelection(const CurveEditorDataModelSelection&, bool)
{
    //to override
}

void CCurveEditorSelectionControllerListenerBase::OnAddedToSelection(const CurveEditorControllerSelection&)
{
    //to override
}

void CCurveEditorSelectionControllerListenerBase::OnRemovedFromSelection(const CurveEditorControllerSelection&, bool)
{
    //to override
}