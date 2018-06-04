#include "pch.h"
#include "SplinePathController.h"
#include "SplineControllerBase.h"

class CCurveEditorSplinePathController final : public CCurveEditorSplineControllerBase<ICurveEditorPathSplineController>
{
public:
    CCurveEditorSplinePathController() = default;
    virtual ~CCurveEditorSplinePathController() override final = default;

protected:
    virtual IEditorListenerUniquePtr CreateListener() override final;
};

class CCurveEditorFunctionPathControllerListener final : public CCurveEditorSplineDataModelListenerBase
{
public:
    CCurveEditorFunctionPathControllerListener(CCurveEditorSplinePathController& pathSplineController);
    virtual ~CCurveEditorFunctionPathControllerListener() override final = default;

    virtual void OnControlPointsAdded(const SplineControlPointsPositions& positions) override final;
    virtual void OnControlPointsRemoved(const SplineControlPointsIndexes& indexes) override final;

private:
    CCurveEditorSplinePathController& m_PathSplineController;
};

CCurveEditorFunctionPathControllerListener::CCurveEditorFunctionPathControllerListener(CCurveEditorSplinePathController& pathSplineController) :
    m_PathSplineController(pathSplineController)
{
}

void CCurveEditorFunctionPathControllerListener::OnControlPointsAdded(const SplineControlPointsPositions& positions)
{
    m_PathSplineController.OnControlPointsAdded(positions);
}

void CCurveEditorFunctionPathControllerListener::OnControlPointsRemoved(const SplineControlPointsIndexes& indexes)
{
    m_PathSplineController.OnControlPointsRemoved(indexes);
}

IEditorListenerUniquePtr CCurveEditorSplinePathController::CreateListener()
{
    return std::make_unique<CCurveEditorFunctionPathControllerListener>(*this);
}

ICurveEditorPathSplineControllerUniquePtr ICurveEditorPathSplineController::Create()
{
    return std::make_unique<CCurveEditorSplinePathController>();
}