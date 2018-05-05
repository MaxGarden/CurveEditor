#include "pch.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"
#include "SplineDataModel.h"
#include "SplineController.h"
#include "CurveEditorListenerBase.h"

class CCurveEditorController;

class CCurveEditorDataModelControllerListener final : public CCurveEditorDataModelListenerBase
{
public:
    CCurveEditorDataModelControllerListener(CCurveEditorController& controller);
    virtual ~CCurveEditorDataModelControllerListener() override final = default;

    virtual void OnSplineCreated(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) override final;
    virtual void OnSplineDestroyed(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) override final;

private:
    CCurveEditorController& m_Controller;
};

class CCurveEditorController final : public CEditorControllerBase<ICurveEditorController, ICurveEditorDataModel, ICurveEditorControllerListener>
{
public:
    CCurveEditorController(ICurveEditorSplineControllerFactory& splineControllerFactory);
    virtual ~CCurveEditorController() override final = default;

    bool CreateSpline(const ICurveEditorSplineDataModelSharedPtr& splineDataModel);
    bool DestroySpline(const ICurveEditorSplineDataModelSharedPtr& splineDataModel);

    virtual bool SetActiveTool(ICurveEditorToolSharedPtr&& tool) noexcept override final;
    virtual const ICurveEditorToolSharedPtr& GetActiveTool() const noexcept override final;

    virtual const ICurveEditorSplineControllerSharedPtr& GetSplineController(const SplineID& id) const noexcept override final;
    virtual void VisitSplineControllers(const ConstVisitorType<ICurveEditorSplineControllerSharedPtr>& visitor) const noexcept override final;

    virtual bool AddToSelection(const SplineID& id, size_t controlPointIndex) override final;
    virtual bool RemoveFromSelection(const SplineID& id, size_t controlPointIndex) override final;
    virtual bool CheckIfSelected(const SplineID& id, size_t controlPointIndex) const noexcept override final;
    virtual void ClearSelection() override final;

    virtual const SCurveEditorStyle& GetEditorStyle() const noexcept override final;

private:
    virtual void OnDataModelChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

    void RecreateSplineControllers();

private:
    std::map<ICurveEditorSplineDataModelConstSharedPtr, ICurveEditorSplineControllerSharedPtr> m_SplineControllers;
    ICurveEditorSplineControllerFactory& m_SplineControllerFactory;
    ICurveEditorToolSharedPtr m_ActiveTool;
    std::map<SplineID, std::set<size_t>> m_SelectedControlPoints;
};

CCurveEditorDataModelControllerListener::CCurveEditorDataModelControllerListener(CCurveEditorController& controller) :
    m_Controller(controller)
{
}

void CCurveEditorDataModelControllerListener::OnSplineCreated(const ICurveEditorSplineDataModelSharedPtr& splineDataModel)
{
    const auto result = m_Controller.CreateSpline(splineDataModel);
    EDITOR_ASSERT(result);
}

void CCurveEditorDataModelControllerListener::OnSplineDestroyed(const ICurveEditorSplineDataModelSharedPtr& splineDataModel)
{
    const auto result = m_Controller.DestroySpline(splineDataModel);
    EDITOR_ASSERT(result);
}

CCurveEditorController::CCurveEditorController(ICurveEditorSplineControllerFactory& splineControllerFactory) :
    m_SplineControllerFactory(splineControllerFactory)
{
}

bool CCurveEditorController::CreateSpline(const ICurveEditorSplineDataModelSharedPtr& splineDataModel)
{
    if (!splineDataModel)
        return false;

    const auto splineController = ICurveEditorSplineControllerSharedPtr(m_SplineControllerFactory.Create(splineDataModel));

    if (!splineController)
        return false;

    auto isValid = true;
    isValid &= splineController->SetDataModel(splineDataModel);

    if (!isValid)
        return false;

    m_SplineControllers.emplace(splineDataModel, splineController);
    NotifyListeners(&ICurveEditorControllerListener::OnSplineCreated, splineController);

    return true;
}

bool CCurveEditorController::DestroySpline(const ICurveEditorSplineDataModelSharedPtr& splineDataModel)
{
    const auto iterator = m_SplineControllers.find(splineDataModel);
    if (iterator == m_SplineControllers.end())
        return false;

    NotifyListeners(&ICurveEditorControllerListener::OnSplineDestroyed, iterator->second);
    m_SplineControllers.erase(iterator);

    return true;
}

const SCurveEditorStyle& CCurveEditorController::GetEditorStyle() const noexcept
{
    static SCurveEditorStyle nullStyle;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetStyle();

    return nullStyle;
}

const ICurveEditorSplineControllerSharedPtr& CCurveEditorController::GetSplineController(const SplineID& id) const noexcept
{
    static const ICurveEditorSplineControllerSharedPtr null;

    const auto dataModel = GetDataModel();
    if (!dataModel)
        return null;

    const auto& splineDataModel = dataModel->GetSplineDataModel(id);
    const auto iterator = m_SplineControllers.find(splineDataModel);

    if (iterator == m_SplineControllers.end())
        return null;

    EDITOR_ASSERT(iterator->second->GetID() == id);
    return iterator->second;
}

void CCurveEditorController::VisitSplineControllers(const ConstVisitorType<ICurveEditorSplineControllerSharedPtr>& visitor) const noexcept
{
    VisitObjectsMap(m_SplineControllers, visitor);
}

bool CCurveEditorController::AddToSelection(const SplineID& id, size_t controlPointIndex)
{
    const auto result = m_SelectedControlPoints[id].emplace(controlPointIndex);
    if (!result.second)
        return false;

    NotifyListeners(&ICurveEditorControllerListener::OnSelectionChanged);
    return true;
}

bool CCurveEditorController::RemoveFromSelection(const SplineID& id, size_t controlPointIndex)
{
    const auto iterator = m_SelectedControlPoints.find(id);
    if (iterator == m_SelectedControlPoints.cend())
        return false;

    auto& controlPointsSet = iterator->second;
    EDITOR_ASSERT(!controlPointsSet.empty());

    const auto removedCount = controlPointsSet.erase(controlPointIndex);
    EDITOR_ASSERT(removedCount <= 1);
    if (removedCount == 0)
        return false;

    if (controlPointsSet.empty())
        m_SelectedControlPoints.erase(iterator);

    NotifyListeners(&ICurveEditorControllerListener::OnSelectionChanged);
    return true;
}

bool CCurveEditorController::CheckIfSelected(const SplineID& id, size_t controlPointIndex) const noexcept
{
    const auto iterator = m_SelectedControlPoints.find(id);
    if (iterator == m_SelectedControlPoints.cend())
        return false;

    const auto& controlPointsSet = iterator->second;
    return controlPointsSet.find(controlPointIndex) == controlPointsSet.cend();
}

void CCurveEditorController::ClearSelection()
{
    if (m_SelectedControlPoints.empty())
        return;

    m_SelectedControlPoints.clear();
    NotifyListeners(&ICurveEditorControllerListener::OnSelectionChanged);
}

void CCurveEditorController::OnDataModelChanged()
{
    RecreateSplineControllers();
}

void CCurveEditorController::RecreateSplineControllers()
{
    for (const auto& splinesPair : m_SplineControllers)
        NotifyListeners(&ICurveEditorControllerListener::OnSplineDestroyed, splinesPair.second);

    m_SplineControllers.clear();

    const auto& dataModel = GetDataModel();
    if (!dataModel)
        return;

    dataModel->VisitSplineDataModels([this](const auto& splineDataModel)
    {
        CreateSpline(splineDataModel);
    });
}

bool CCurveEditorController::SetActiveTool(ICurveEditorToolSharedPtr&& tool) noexcept
{
    m_ActiveTool = std::move(tool);
    return true;
}

const ICurveEditorToolSharedPtr& CCurveEditorController::GetActiveTool() const noexcept
{
    return m_ActiveTool;
}

IEditorListenerUniquePtr CCurveEditorController::CreateListener()
{
    return std::make_unique<CCurveEditorDataModelControllerListener>(*this);
}

ICurveEditorControllerUniquePtr ICurveEditorController::Create(ICurveEditorSplineControllerFactory& splineControllerFactory)
{
    return std::make_unique<CCurveEditorController>(splineControllerFactory);
}