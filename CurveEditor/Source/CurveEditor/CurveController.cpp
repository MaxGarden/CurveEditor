#include "pch.h"
#include "CurveController.h"
#include "SplineComponentControllerBase.h"

class CCurveEditorCurveControllerPrivate final : public CCurveEditorSplineComponentControllerBase<ICurveEditorCurveControllerPrivate, ECurveEditorSplineComponentType::Curve>
{
public:
    CCurveEditorCurveControllerPrivate() = default;
    virtual ~CCurveEditorCurveControllerPrivate() override final = default;

    virtual bool VisitCurvePoints(const ConstVisitorType<ax::pointf>& visitor) const noexcept override final;
    virtual const SplineColor& GetColor() const noexcept override final;

    virtual bool SetCurveIndex(size_t curveIndex) noexcept override final;
    virtual std::optional<size_t> GetIndex() const noexcept override final;

private:
    std::optional<size_t> m_CurveIndex;
    std::optional<std::pair<size_t, size_t>> m_ControlPointsRange;
};

bool CCurveEditorCurveControllerPrivate::VisitCurvePoints(const ConstVisitorType<ax::pointf>& visitor) const noexcept
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

    const auto& controlPoints = GetControlPoints();
    EDITOR_ASSERT(lastControlPointIndex <= controlPoints.size());
    if (lastControlPointIndex > controlPoints.size())
        return false;

    for (auto i = firstControlPointIndex; i < lastControlPointIndex; ++i)
        visitor(controlPoints[i]);

    return true;
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

ICurveEditorCurveControllerPrivateUniquePtr ICurveEditorCurveControllerPrivate::Create()
{
    return std::make_unique<CCurveEditorCurveControllerPrivate>();
}