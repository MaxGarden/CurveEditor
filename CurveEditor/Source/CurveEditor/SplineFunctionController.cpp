#include "pch.h"
#include "SplineFunctionController.h"

const std::string& CCurveEditorFunctionSplineController::GetName() const noexcept
{
    static const std::string null;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetName();

    return null;
}

void CCurveEditorFunctionSplineController::OnSplineModified() noexcept
{
    const auto& dataModel = GetDataModel();
    if(!dataModel)
        return;

    SortControlPoints(dataModel->GetControlPoints());
}

void CCurveEditorFunctionSplineController::OnDataModelChanged()
{
    OnSplineModified();
}

void CCurveEditorFunctionSplineController::SortControlPoints(std::vector<ax::pointf>& controlPoints) noexcept
{
    static const auto prediction = [](const auto& first, const auto& second)
    {
        return first.x < second.x;
    };

    std::sort(controlPoints.begin(), controlPoints.end(), prediction);
}