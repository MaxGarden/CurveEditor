#include "pch.h"
#include "TangentController.h"
#include "SplineComponentControllerBase.h"

class CCurveEditorTangentControllerPrivate final : public CCurveEditorSplineComponentControllerBase<ICurveEditorTangentControllerPrivate, ECurveEditorSplineComponentType::Tangent>
{
public:
    CCurveEditorTangentControllerPrivate() = default;
    virtual ~CCurveEditorTangentControllerPrivate() override final = default;

    virtual bool SetPosition(const ax::pointf& position) override final;
    virtual std::optional<ax::pointf> GetPosition() const noexcept override final;

    virtual std::optional<ax::pointf> GetAnchorPosition() const noexcept override final;

    virtual bool SetTangentIndex(size_t tangentIndex) noexcept override final;
    virtual std::optional<size_t> GetControlPointIndex() const noexcept override final;

    virtual std::optional<size_t> GetIndex() const noexcept override final;

private:
    std::optional<size_t> m_TangentIndex;
    std::optional<size_t> m_TangentControlPointIndex;
    std::optional<size_t> m_AnchorControlPointIndex;
};

bool CCurveEditorTangentControllerPrivate::SetPosition(const ax::pointf& position)
{
    EDITOR_ASSERT(m_TangentControlPointIndex);
    if (!m_TangentControlPointIndex)
        return false;

    return SetControlPoints({ { *m_TangentControlPointIndex, position } });
}

std::optional<ax::pointf> CCurveEditorTangentControllerPrivate::GetPosition() const noexcept
{
    EDITOR_ASSERT(m_TangentControlPointIndex);
    if (!m_TangentControlPointIndex)
        return std::nullopt;

    return GetControlPoint(*m_TangentControlPointIndex);
}

std::optional<ax::pointf> CCurveEditorTangentControllerPrivate::GetAnchorPosition() const noexcept
{
    EDITOR_ASSERT(m_AnchorControlPointIndex);
    if (!m_AnchorControlPointIndex)
        return std::nullopt;

    return GetControlPoint(*m_AnchorControlPointIndex);
}

bool CCurveEditorTangentControllerPrivate::SetTangentIndex(size_t tangentIndex) noexcept
{
    if (m_TangentIndex.has_value() && *m_TangentIndex == tangentIndex)
        return true;

    const auto controlPointsPerCurveMinusOne = (ICurveEditorSplineController::ControlPointsPerCurve() - 1);
    const auto tangentControlPointIndex = 1 + (tangentIndex / 2) * controlPointsPerCurveMinusOne + tangentIndex % 2;

    const auto& controlPoints = GetControlPoints();
    EDITOR_ASSERT(tangentControlPointIndex < controlPoints.size());
    if (tangentControlPointIndex >= controlPoints.size())
        return false;

    const auto anchorControlPointIndex = (tangentIndex == 0) ? 0 : (tangentIndex + 1) / 2 * controlPointsPerCurveMinusOne;
    EDITOR_ASSERT(anchorControlPointIndex < controlPoints.size());
    if (anchorControlPointIndex >= controlPoints.size())
        return false;

    m_TangentIndex = tangentIndex;
    m_TangentControlPointIndex = tangentControlPointIndex;
    m_AnchorControlPointIndex = anchorControlPointIndex;
    return true;
}

std::optional<size_t> CCurveEditorTangentControllerPrivate::GetIndex() const noexcept
{
    return m_TangentIndex;
}

std::optional<size_t> CCurveEditorTangentControllerPrivate::GetControlPointIndex() const noexcept
{
    return m_TangentControlPointIndex;
}

ICurveEditorTangentControllerPrivateUniquePtr ICurveEditorTangentControllerPrivate::Create()
{
    return std::make_unique<CCurveEditorTangentControllerPrivate>();
}