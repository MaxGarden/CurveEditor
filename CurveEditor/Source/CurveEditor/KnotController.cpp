#include "pch.h"
#include "KnotController.h"
#include "SplineController.h"
#include "EditorControllerBase.h"

class CCurveEditorKnotControllerPrivate final : public CEditorControllerBase<ICurveEditorKnotControllerPrivate, ICurveEditorSplineDataModel, IEditorListener>
{
public:
    CCurveEditorKnotControllerPrivate() = default;
    virtual ~CCurveEditorKnotControllerPrivate() override final = default;

    virtual std::optional<ax::pointf> GetPosition() const noexcept override final;

    virtual bool SetKnotIndex(size_t knotIndex) noexcept override final;

private:
    std::optional<size_t> m_ControlPointIndex;
};

std::optional<ax::pointf> CCurveEditorKnotControllerPrivate::GetPosition() const noexcept
{
    EDITOR_ASSERT(m_ControlPointIndex);
    if (!m_ControlPointIndex)
        return std::nullopt;

    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return std::nullopt;

    const auto& controlPoints = dataModel->GetControlPoints();
    EDITOR_ASSERT(*m_ControlPointIndex < controlPoints.size());
    if (*m_ControlPointIndex >= controlPoints.size())
        return std::nullopt;

    return controlPoints[*m_ControlPointIndex];
}

bool CCurveEditorKnotControllerPrivate::SetKnotIndex(size_t knotIndex) noexcept
{
    const auto controlPointIndex = knotIndex * (ICurveEditorSplineController::ControlPointsPerCurve() - 1);

    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    const auto& controlPoints = dataModel->GetControlPoints();
    EDITOR_ASSERT(controlPointIndex < controlPoints.size());
    if (controlPointIndex >= controlPoints.size())
        return false;

    m_ControlPointIndex = controlPointIndex;
    return true;
}

ICurveEditorKnotControllerPrivateUniquePtr ICurveEditorKnotControllerPrivate::Create()
{
    return std::make_unique<CCurveEditorKnotControllerPrivate>();
}