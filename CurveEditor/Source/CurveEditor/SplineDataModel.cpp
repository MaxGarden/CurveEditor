#include "pch.h"
#include "SplineDataModel.h"

std::vector<ax::pointf>& CCurveEditorSplineDataModel::GetControlPoints() noexcept
{
    return m_ControlPoints;
}