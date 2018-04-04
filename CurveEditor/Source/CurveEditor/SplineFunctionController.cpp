#include "pch.h"
#include "SplineFunctionController.h"

static const auto s_ControlPointsPerCurve = 4u;

const std::string& CCurveEditorFunctionSplineController::GetName() const noexcept
{
    static const std::string null;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetName();

    return null;
}

unsigned int CCurveEditorFunctionSplineController::GetColor() const noexcept
{
    if (const auto dataModel = GetDataModel())
        return dataModel->GetColor();

    return 0;
}

const std::vector<ax::pointf>& CCurveEditorFunctionSplineController::GetControlPoints() const noexcept
{
    static const std::vector<ax::pointf> null;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetControlPoints();

    return null;
}

bool CCurveEditorFunctionSplineController::VisitCurvePoints(size_t curveIndex, const CurveConstVisitor& visitor) const noexcept
{
    if (!visitor)
        return false;

    const auto& controlPoints = GetControlPoints();
    const auto actualIndex = curveIndex * (s_ControlPointsPerCurve - 1);

    if (actualIndex >= controlPoints.size())
        return false;

    const auto endIndex = actualIndex + s_ControlPointsPerCurve;

    for (auto i = actualIndex; i < endIndex; ++i)
        visitor(controlPoints[i]);

    return true;
}

size_t CCurveEditorFunctionSplineController::GetCurvesCount() const noexcept
{
    const auto controlPointsCount = GetControlPoints().size();
    if (controlPointsCount < s_ControlPointsPerCurve)
        return 0;

    return 1 + (controlPointsCount - s_ControlPointsPerCurve) / (s_ControlPointsPerCurve - 1);
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
    const auto actualIndex = knotIndex * (s_ControlPointsPerCurve - 1);

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