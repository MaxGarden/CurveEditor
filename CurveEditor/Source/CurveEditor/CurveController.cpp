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

    virtual bool VisitCurveControlPoints(const ConstVisitorType<ax::pointf>& visitor) const override final;
    virtual const SplineColor& GetColor() const noexcept override final;

    virtual bool SetCurveIndex(size_t curveIndex) noexcept override final;
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

ICurveEditorCurveControllerPrivateUniquePtr ICurveEditorCurveControllerPrivate::Create()
{
    return std::make_unique<CCurveEditorCurveControllerPrivate>();
}