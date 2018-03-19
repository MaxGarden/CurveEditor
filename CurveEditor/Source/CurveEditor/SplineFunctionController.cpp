#include "pch.h"
#include "SplineFunctionController.h"
#include "SplineDataModel.h"

bool CCurveEditorFunctionSplineController::SetDataModel(const IEditorDataModelSharedPtr& dataModel) noexcept
{
    if (!dataModel)
    {
        m_DataModel.reset();
        return true;
    }

    auto splineDataModel = std::dynamic_pointer_cast<ICurveEditorSplineDataModel>(dataModel);
    if (!splineDataModel)
        return false;

    m_DataModel = std::move(splineDataModel);
    return true;
}

void CCurveEditorFunctionSplineController::OnSplineModified() noexcept
{
    if (!m_DataModel)
        return;

    SortControlPoints(m_DataModel->GetControlPoints());
}

void CCurveEditorFunctionSplineController::SortControlPoints(std::vector<ax::pointf>& controlPoints) noexcept
{
    static const auto prediction = [](const auto& first, const auto& second)
    {
        return first.x < second.x;
    };

    std::sort(controlPoints.begin(), controlPoints.end(), prediction);
}

const std::string& CCurveEditorFunctionSplineController::GetName() const noexcept
{
    static const std::string null;

    if (m_DataModel)
        return m_DataModel->GetName();

    return null;
}