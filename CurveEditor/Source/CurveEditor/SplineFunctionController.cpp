#include "pch.h"
#include "SplineFunctionController.h"

const std::string& CCurveEditorFunctionSplineController::GetName() const noexcept
{
    static const std::string null;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetName();

    return null;
}

const std::vector<ax::pointf>& CCurveEditorFunctionSplineController::GetControlPoints() const noexcept
{
    static const std::vector<ax::pointf> null;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetControlPoints();

    return null;
}

bool CCurveEditorFunctionSplineController::VisitCurvePoints(size_t index, const CurveConstVisitor& visitor) const noexcept
{
    if (!visitor)
        return false;

    const auto& controlPoints = GetControlPoints();
    const auto actualIndex = index * 4;

    if (actualIndex >= controlPoints.size())
        return false;

    const auto endIndex = actualIndex + 4;

    for (auto i = actualIndex; i < endIndex; ++i)
        visitor(controlPoints[i]);

    return true;
}

size_t CCurveEditorFunctionSplineController::GetCurvesCount() const noexcept
{
    const auto controlPointsCount = GetControlPoints().size();
    if (controlPointsCount < 4)
        return 0;

    return GetControlPoints().size() / 4 + 1;
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

std::optional<ax::pointf> CCurveEditorFunctionSplineController::GetKnot(size_t knotIndex) const noexcept
{
    const auto actualIndex = knotIndex * 3;

    const auto& controlPoints = GetControlPoints();
    if (actualIndex >= controlPoints.size())
        return std::nullopt;

    return controlPoints[actualIndex];
}

size_t CCurveEditorFunctionSplineController::GetKnotsCount() const noexcept
{
    const auto curvesCount = GetCurvesCount();
    if (curvesCount == 0)
        return 0;

    return curvesCount + 1;
}