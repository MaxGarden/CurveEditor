#include "pch.h"
#include "CurveEditorDataModel.h"
#include "SplineDataModel.h"

SEditorStyle& CCurveEditorDataModel::GetStyle() noexcept
{
    return m_EditorStyle;
}

const SEditorStyle& CCurveEditorDataModel::GetStyle() const noexcept
{
    return m_EditorStyle;
}

bool CCurveEditorDataModel::AddSplineDataModel(const CCurveEditorSplineDataModelSharedPtr& splineDataModel)
{
	if (!splineDataModel)
		return false;

	m_SplinesDataModels.emplace_back(splineDataModel);
	return true;
}

bool CCurveEditorDataModel::RemoveSplineDataModel(const CCurveEditorSplineDataModelSharedPtr& splineDataModel)
{
	const auto iterator = std::remove(m_SplinesDataModels.begin(), m_SplinesDataModels.end(), splineDataModel);

	if (iterator == m_SplinesDataModels.end())
		return false;

	m_SplinesDataModels.erase(iterator, m_SplinesDataModels.end());
	return true;
}

const std::vector<CCurveEditorSplineDataModelSharedPtr>& CCurveEditorDataModel::GetSplinesDataModels() const noexcept
{
	return m_SplinesDataModels;
}