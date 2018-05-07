#include "pch.h"
#include "TangentController.h"
#include "SplineController.h"
#include "EditorControllerBase.h"

class CCurveEditorTangentControllerPrivate final : public CEditorControllerBase<ICurveEditorTangentControllerPrivate, ICurveEditorSplineDataModel, IEditorListener>
{
public:
    CCurveEditorTangentControllerPrivate() = default;
    virtual ~CCurveEditorTangentControllerPrivate() override final = default;

    virtual std::optional<ax::pointf> GetPosition() const noexcept override final;
    virtual std::optional<ax::pointf> GetAnchorPosition() const noexcept override final;

    virtual bool SetTangentIndex(size_t tangentIndex) noexcept override final;

private:
    std::optional<ax::pointf> GetControlPointPosition(const std::optional<size_t>& controlPoint) const noexcept;

private:
    std::optional<size_t> m_TangentControlPointIndex;
    std::optional<size_t> m_AnchorControlPointIndex;
};

std::optional<ax::pointf> CCurveEditorTangentControllerPrivate::GetPosition() const noexcept
{
    return GetControlPointPosition(m_TangentControlPointIndex);
}

std::optional<ax::pointf> CCurveEditorTangentControllerPrivate::GetAnchorPosition() const noexcept
{
    return GetControlPointPosition(m_AnchorControlPointIndex);
}

bool CCurveEditorTangentControllerPrivate::SetTangentIndex(size_t tangentIndex) noexcept
{
    const auto controlPointsPerCurveMinusOne = (ICurveEditorSplineController::ControlPointsPerCurve() - 1);
    const auto tangentControlPointIndex = 1 + (tangentIndex / 2) * controlPointsPerCurveMinusOne + tangentIndex % 2;

    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    const auto& controlPoints = dataModel->GetControlPoints();
    EDITOR_ASSERT(tangentControlPointIndex < controlPoints.size());
    if (tangentControlPointIndex >= controlPoints.size())
        return false;

    const auto anchorControlPointIndex = (tangentIndex == 0) ? 0 : (tangentIndex + 1) / 2 * controlPointsPerCurveMinusOne;
    EDITOR_ASSERT(anchorControlPointIndex < controlPoints.size());
    if (anchorControlPointIndex >= controlPoints.size())
        return false;

    m_TangentControlPointIndex = tangentControlPointIndex;
    m_AnchorControlPointIndex = anchorControlPointIndex;
    return true;
}

std::optional<ax::pointf> CCurveEditorTangentControllerPrivate::GetControlPointPosition(const std::optional<size_t>& controlPoint) const noexcept
{
    EDITOR_ASSERT(controlPoint);
    if (!controlPoint)
        return std::nullopt;

    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return std::nullopt;

    const auto& controlPoints = dataModel->GetControlPoints();
    EDITOR_ASSERT(*m_TangentControlPointIndex < controlPoints.size());
    if (*m_TangentControlPointIndex >= controlPoints.size())
        return std::nullopt;

    return controlPoints[*controlPoint];
}

ICurveEditorTangentControllerPrivateUniquePtr ICurveEditorTangentControllerPrivate::Create()
{
    return std::make_unique<CCurveEditorTangentControllerPrivate>();
}