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

template<typename ContainerType, typename ValueType>
static const auto RemoveFromContainer(ContainerType& container, const ValueType& value)
{
    const auto iterator = std::remove(container.begin(), container.end(), value);

    if (iterator == container.end())
        return false;

    container.erase(iterator, container.end());
    return true;
}

ICurveEditorSplineDataModelSharedPtr CCurveEditorDataModel::AddSplineDataModel(std::string&& name)
{
    auto splineDataModel = ICurveEditorSplineDataModel::Create(std::move(name));
    if (!splineDataModel)
        return nullptr;

    m_SplinesDataModels.emplace_back(std::move(splineDataModel));
    return m_SplinesDataModels.back();
}

bool CCurveEditorDataModel::RemoveSplineDataModel(const ICurveEditorSplineDataModelSharedPtr& splineDataModel)
{
    return RemoveFromContainer(m_SplinesDataModels, splineDataModel);
}

const std::vector<ICurveEditorSplineDataModelSharedPtr>& CCurveEditorDataModel::GetSplinesDataModels() const noexcept
{
    return m_SplinesDataModels;
}