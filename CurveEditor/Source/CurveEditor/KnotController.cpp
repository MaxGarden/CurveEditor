#include "pch.h"
#include "KnotController.h"
#include "SplineComponentControllerBase.h"

class CCurveEditorKnotControllerPrivate final : public CCurveEditorSplineComponentControllerBase<ICurveEditorKnotControllerPrivate, ECurveEditorSplineComponentType::Knot>
{
public:
    CCurveEditorKnotControllerPrivate() = default;
    virtual ~CCurveEditorKnotControllerPrivate() override final = default;

    virtual std::optional<ax::pointf> GetPosition() const noexcept override final;
    virtual bool SetPosition(const ax::pointf& position) override final;

    virtual bool SetKnotIndex(size_t knotIndex) noexcept override final;
    virtual std::optional<size_t> GetControlPointIndex() const noexcept override final;

    virtual std::optional<size_t> GetIndex() const noexcept override final;

    virtual bool CanBeRemoved() const noexcept override final;
    virtual bool Remove() override final;

private:
    std::optional<size_t> m_KnotIndex;
    std::optional<size_t> m_ControlPointIndex;
};

bool CCurveEditorKnotControllerPrivate::SetPosition(const ax::pointf& position)
{
    EDITOR_ASSERT(m_ControlPointIndex);
    if (!m_ControlPointIndex)
        return false;

    const auto knotControlPointIndex = *m_ControlPointIndex;

    const auto knotControlPointPosition = GetPosition();
    EDITOR_ASSERT(knotControlPointPosition);
    if (!knotControlPointPosition)
        return false;

    const auto delta = position - *knotControlPointPosition;

    SplineControlPointsPositions controlPointsDeltaPositions;

    const auto& controlPoints = GetControlPoints();

    controlPointsDeltaPositions.emplace(knotControlPointIndex, delta);

    if (const auto controlPointIndex = knotControlPointIndex - 1; controlPointIndex < controlPoints.size())
        controlPointsDeltaPositions.emplace(controlPointIndex, delta);

    if (const auto controlPointIndex = knotControlPointIndex + 1; controlPointIndex < controlPoints.size())
        controlPointsDeltaPositions.emplace(controlPointIndex, delta);

    return MoveControlPoints(controlPointsDeltaPositions);
}

std::optional<ax::pointf> CCurveEditorKnotControllerPrivate::GetPosition() const noexcept
{
    EDITOR_ASSERT(m_ControlPointIndex);
    if (!m_ControlPointIndex)
        return std::nullopt;

    return GetControlPoint(*m_ControlPointIndex);
}

bool CCurveEditorKnotControllerPrivate::SetKnotIndex(size_t knotIndex) noexcept
{
    if (m_KnotIndex.has_value() && *m_KnotIndex == knotIndex)
        return true;

    const auto controlPointIndex = knotIndex * (ICurveEditorSplineController::ControlPointsPerCurve() - 1);

    const auto& controlPoints = GetControlPoints();
    EDITOR_ASSERT(controlPointIndex < controlPoints.size());
    if (controlPointIndex >= controlPoints.size())
        return false;

    m_KnotIndex = knotIndex;
    m_ControlPointIndex = controlPointIndex;
    return true;
}

std::optional<size_t> CCurveEditorKnotControllerPrivate::GetControlPointIndex() const noexcept
{
    return m_ControlPointIndex;
}

std::optional<size_t> CCurveEditorKnotControllerPrivate::GetIndex() const noexcept
{
    return m_KnotIndex;
}

bool CCurveEditorKnotControllerPrivate::CanBeRemoved() const noexcept
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    const auto controlPointsCount = dataModel->GetControlPoints().size();

    return controlPointsCount - 2 > ICurveEditorSplineDataModel::ControlPointsPerCurve();
}

bool CCurveEditorKnotControllerPrivate::Remove()
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    const auto optionalControlPointIndex = GetControlPointIndex();
    EDITOR_ASSERT(optionalControlPointIndex);
    if (!optionalControlPointIndex)
        return false;

    SplineControlPointsIndexes controlPointsIndexesToRemove = { *optionalControlPointIndex };

    const auto& controlPointsCount = dataModel->GetControlPoints().size();
    const auto tryRemoveTangentControlPoint = [controlPointsCount, &controlPointsIndexesToRemove](const auto controlPointIndex)
    {
        if (controlPointIndex < controlPointsCount)
        {
            controlPointsIndexesToRemove.emplace(controlPointIndex);
            return true;
        }

        return false;
    };

    if (!tryRemoveTangentControlPoint(*optionalControlPointIndex - 1))
    {
        const auto result = tryRemoveTangentControlPoint(*optionalControlPointIndex + 2);
        EDITOR_ASSERT(result);
    }

    if (!tryRemoveTangentControlPoint(*optionalControlPointIndex + 1))
    {
        const auto result = tryRemoveTangentControlPoint(*optionalControlPointIndex - 2);
        EDITOR_ASSERT(result);
    }

    return dataModel->RemoveControlPoints(controlPointsIndexesToRemove);
}

ICurveEditorKnotControllerPrivateUniquePtr ICurveEditorKnotControllerPrivate::Create()
{
    return std::make_unique<CCurveEditorKnotControllerPrivate>();
}