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
    virtual std::optional<size_t> GetIndex() const noexcept override final;

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

    const auto moveTangent = [this, &delta](const auto tangentControlPointIndex)
    {
        MoveControlPointPosition(tangentControlPointIndex, delta);
    };

    moveTangent(knotControlPointIndex - 1);
    moveTangent(knotControlPointIndex + 1);

    return SetControlPointPosition(knotControlPointIndex, position);
}

std::optional<ax::pointf> CCurveEditorKnotControllerPrivate::GetPosition() const noexcept
{
    EDITOR_ASSERT(m_ControlPointIndex);
    if (!m_ControlPointIndex)
        return std::nullopt;

    return GetControlPointPosition(*m_ControlPointIndex);
}

bool CCurveEditorKnotControllerPrivate::SetKnotIndex(size_t knotIndex) noexcept
{
    const auto controlPointIndex = knotIndex * (ICurveEditorSplineController::ControlPointsPerCurve() - 1);

    const auto& controlPoints = GetControlPoints();
    EDITOR_ASSERT(controlPointIndex < controlPoints.size());
    if (controlPointIndex >= controlPoints.size())
        return false;

    m_KnotIndex = knotIndex;
    m_ControlPointIndex = controlPointIndex;
    return true;
}

std::optional<size_t> CCurveEditorKnotControllerPrivate::GetIndex() const noexcept
{
    return m_KnotIndex;
}

ICurveEditorKnotControllerPrivateUniquePtr ICurveEditorKnotControllerPrivate::Create()
{
    return std::make_unique<CCurveEditorKnotControllerPrivate>();
}