#include "pch.h"
#include "SplineDataModel.h"

CCurveEditorSplineDataModel::CCurveEditorSplineDataModel(std::string&& name) :
	m_Name(std::move(name))
{
}

std::vector<ax::pointf>& CCurveEditorSplineDataModel::GetControlPoints() noexcept
{
    return m_ControlPoints;
}

const std::string& CCurveEditorSplineDataModel::GetName() const noexcept
{
	return m_Name;
}