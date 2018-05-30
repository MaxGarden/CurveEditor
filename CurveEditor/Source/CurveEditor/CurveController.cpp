#include "pch.h"
#include "CurveController.h"
#include "SplineComponentControllerBase.h"

class CCurveEditorCurveControllerPrivate final : public CCurveEditorSplineComponentControllerBase<ICurveEditorCurveControllerPrivate, ECurveEditorSplineComponentType::Curve>
{
public:
    CCurveEditorCurveControllerPrivate() = default;
    virtual ~CCurveEditorCurveControllerPrivate() override final = default;

    virtual bool SetPosition(const ax::pointf& position) override final;
    virtual std::optional<ax::pointf> GetPosition() const noexcept override final;

    virtual bool InsertKnot(float tPosition) override final;

    virtual bool VisitCurveControlPoints(const ConstVisitorType<ax::pointf>& visitor) const override final;
    virtual const SplineColor& GetColor() const noexcept override final;

    virtual bool SetCurveIndex(size_t curveIndex) noexcept override final;
    virtual std::optional<size_t> GetControlPointIndex() const noexcept override final;

    virtual std::optional<size_t> GetIndex() const noexcept override final;

private:
    bool VisitCurveControlPointIndexes(const ConstVisitorType<size_t>& visitor) const;

private:
    std::optional<size_t> m_CurveIndex;
    std::optional<std::pair<size_t, size_t>> m_ControlPointsRange;
};

bool CCurveEditorCurveControllerPrivate::SetPosition(const ax::pointf& position)
{
    const auto currentPosition = GetPosition();
    EDITOR_ASSERT(currentPosition);
    if (!currentPosition)
        return false;

    const auto delta = position - *currentPosition;
    EDITOR_ASSERT(m_ControlPointsRange);

    std::pair<size_t, size_t> controlPointIndexesRange = { UINT_MAX, 0 };

    SplineControlPointsPositions controlPointsDeltaPositions;

    VisitCurveControlPointIndexes([&delta, &controlPointIndexesRange, &controlPointsDeltaPositions](const auto& controlPointIndex)
    {
        controlPointIndexesRange.first = std::min(controlPointIndexesRange.first, controlPointIndex);
        controlPointIndexesRange.second = std::max(controlPointIndexesRange.second, controlPointIndex);

        controlPointsDeltaPositions.emplace(controlPointIndex, delta);
    });

    const auto& controlPoints = GetControlPoints();

    if (const auto controlPointIndex = controlPointIndexesRange.first - 1; controlPointIndex < controlPoints.size())
        controlPointsDeltaPositions.emplace(controlPointIndex, delta);

    if (const auto controlPointIndex = controlPointIndexesRange.second + 1; controlPointIndex < controlPoints.size())
        controlPointsDeltaPositions.emplace(controlPointIndex, delta);

    return MoveControlPoints(controlPointsDeltaPositions);
}

std::optional<ax::pointf> CCurveEditorCurveControllerPrivate::GetPosition() const noexcept
{
    EDITOR_ASSERT(m_ControlPointsRange);
    if (!m_ControlPointsRange)
        return std::nullopt;

    const auto firstControlPointIndex = m_ControlPointsRange->first;
    const auto& controlPoints = GetControlPoints();

    EDITOR_ASSERT(firstControlPointIndex < controlPoints.size());
    if (firstControlPointIndex >= controlPoints.size())
        return std::nullopt;

    return controlPoints[firstControlPointIndex];
}

bool CCurveEditorCurveControllerPrivate::VisitCurveControlPoints(const ConstVisitorType<ax::pointf>& visitor) const
{
    if (!visitor)
        return false;

    return VisitCurveControlPointIndexes([&visitor, &controlPoints = GetControlPoints()](const auto controlPointIndex)
    {
        EDITOR_ASSERT(controlPointIndex < controlPoints.size());
        if (controlPointIndex >= controlPoints.size())
            return;

        visitor(controlPoints[controlPointIndex]);
    });
}

const SplineColor& CCurveEditorCurveControllerPrivate::GetColor() const noexcept
{
    static const SplineColor null = {};
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return null;

    return dataModel->GetColor();
}

bool CCurveEditorCurveControllerPrivate::SetCurveIndex(size_t curveIndex) noexcept
{
    if (m_CurveIndex.has_value() && *m_CurveIndex == curveIndex)
        return true;

    const auto& controlPoints = GetControlPoints();

    const auto controlPointsPerCurve = ICurveEditorSplineController::ControlPointsPerCurve();
    const auto firstControlPointIndex = curveIndex * (controlPointsPerCurve - 1);
    const auto lastControlPointIndex = firstControlPointIndex + controlPointsPerCurve;

    EDITOR_ASSERT(lastControlPointIndex <= controlPoints.size());
    if (lastControlPointIndex > controlPoints.size())
        return false;

    m_CurveIndex = curveIndex;
    m_ControlPointsRange = { firstControlPointIndex, lastControlPointIndex };
    return true;
}

std::optional<size_t> CCurveEditorCurveControllerPrivate::GetIndex() const noexcept
{
    return m_CurveIndex;
}

bool CCurveEditorCurveControllerPrivate::InsertKnot(float tPosition)
{
    const auto isTPositionValid = tPosition >= 0.0f && tPosition <= 1.0f;
    EDITOR_ASSERT(isTPositionValid);
    if (!isTPositionValid)
        return false;

    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    std::array<ax::pointf, 4u> controlPoints;

    VisitCurveControlPoints([iterator = controlPoints.begin(), endIterator = controlPoints.end()](const auto& point) mutable
    {
        EDITOR_ASSERT(iterator != endIterator);
        if (iterator != endIterator)
            *(iterator++) = point;
    });

    const auto previousTangentPosition = (1.0f - tPosition) * controlPoints[0] + tPosition * controlPoints[1];
    const auto nextTangentPosition = (1.0f - tPosition) * controlPoints[2] + tPosition * controlPoints[3];

    const auto tangentPositionFactor = (1.0f - tPosition) * controlPoints[1] + tPosition * controlPoints[2];

    const auto leftTangentPosition = (1.0f - tPosition) * previousTangentPosition + tPosition * tangentPositionFactor;
    const auto rightTangentPosition = (1.0f - tPosition) * tangentPositionFactor + tPosition * nextTangentPosition;

    const auto knotPosition = ax::cubic_bezier(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], tPosition);

    const auto firstControlPointIndex = GetControlPointIndex();
    EDITOR_ASSERT(firstControlPointIndex);
    if (!firstControlPointIndex)
        return false;

    const auto firstInsertControlPointIndex = *firstControlPointIndex + 2u;

    auto result = true;
    result &= dataModel->AddControlPoints({
        { firstInsertControlPointIndex, leftTangentPosition },
        { firstInsertControlPointIndex + 1, knotPosition },
        { firstInsertControlPointIndex + 2, rightTangentPosition } });

    EDITOR_ASSERT(result);
    if (!result)
        return result;

    const auto previousTangentControlPointIndex = firstInsertControlPointIndex - 1;
    const auto nextTangentControlPointIndex = firstInsertControlPointIndex + 3;

    const auto controlPointsCount = dataModel->GetControlPoints().size();

    const auto isFirstInsertControlPointIndexValid = previousTangentControlPointIndex < controlPointsCount && nextTangentControlPointIndex < controlPointsCount;
    EDITOR_ASSERT(isFirstInsertControlPointIndexValid);
    if (!isFirstInsertControlPointIndexValid)
        return result;

    result &= dataModel->SetControlPoints({
        { previousTangentControlPointIndex, previousTangentPosition },
        { nextTangentControlPointIndex, nextTangentPosition } });

    EDITOR_ASSERT(result);
    return result;
}

bool CCurveEditorCurveControllerPrivate::VisitCurveControlPointIndexes(const ConstVisitorType<size_t>& visitor) const
{
    if (!visitor)
        return false;

    EDITOR_ASSERT(m_ControlPointsRange);
    if (!m_ControlPointsRange)
        return false;

    const auto firstControlPointIndex = m_ControlPointsRange->first;
    const auto lastControlPointIndex = m_ControlPointsRange->second;

    EDITOR_ASSERT(firstControlPointIndex < lastControlPointIndex);
    if (firstControlPointIndex >= lastControlPointIndex)
        return false;

    for (auto i = firstControlPointIndex; i < lastControlPointIndex; ++i)
        visitor(i);

    return true;
}

std::optional<size_t> CCurveEditorCurveControllerPrivate::GetControlPointIndex() const noexcept
{
    if (!m_ControlPointsRange)
        return std::nullopt;

    return m_ControlPointsRange->first;
}

ICurveEditorCurveControllerPrivateUniquePtr ICurveEditorCurveControllerPrivate::Create()
{
    return std::make_unique<CCurveEditorCurveControllerPrivate>();
}