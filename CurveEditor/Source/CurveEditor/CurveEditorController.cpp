#include "pch.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"
#include "SplineDataModel.h"
#include "SplineController.h"
#include "CurveEditorListenerBase.h"
#include "CurveEditorSplineListenerBase.h"
#include "KnotController.h"
#include "TangentController.h"
#include "CurveController.h"
#include "CurveEditorSelectionController.h"

class CCurveEditorController;

class CCurveEditorDataModelControllerListener final : public CCurveEditorDataModelListenerBase
{
public:
    CCurveEditorDataModelControllerListener(CCurveEditorController& controller);
    virtual ~CCurveEditorDataModelControllerListener() override final = default;

    virtual void OnSplineCreated(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) override final;
    virtual void OnSplineDestroyed(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) override final;

    virtual void OnSelectionDataModelChanged(const ICurveEditorSelectionDataModelSharedPtr& selectionDataModel) override final;

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

    void SetSelectionDataModel(const ICurveEditorSelectionDataModelSharedPtr& selectionDataModel);

    virtual bool SetActiveTool(ICurveEditorToolSharedPtr&& tool) noexcept override final;
    virtual const ICurveEditorToolSharedPtr& GetActiveTool() const noexcept override final;

    virtual ICurveEditorSelectionControllerSharedPtr GetSelectionController() const noexcept override final;

    virtual const ICurveEditorSplineControllerSharedPtr& GetSplineController(const SplineID& id) const noexcept override final;
    virtual void VisitSplineControllers(const ConstVisitorType<ICurveEditorSplineControllerSharedPtr>& visitor) const noexcept override final;

    virtual const SCurveEditorStyle& GetEditorStyle() const noexcept override final;

private:
    virtual void OnDataModelChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

    void NotifySplineControllerCreated(const ICurveEditorSplineControllerSharedPtr& splineController);
    void NotifySplineControllerDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController);

    void RecreateSplineControllers();

private:
    std::map<ICurveEditorSplineDataModelConstSharedPtr, ICurveEditorSplineControllerSharedPtr> m_SplineControllers;
    const ICurveEditorSelectionControllerPrivateSharedPtr m_SelectionController;
    ICurveEditorSplineControllerFactory& m_SplineControllerFactory;
    ICurveEditorToolSharedPtr m_ActiveTool;
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

void CCurveEditorDataModelControllerListener::OnSelectionDataModelChanged(const ICurveEditorSelectionDataModelSharedPtr& selectionDataModel)
{
    m_Controller.SetSelectionDataModel(selectionDataModel);
}

CCurveEditorController::CCurveEditorController(ICurveEditorSplineControllerFactory& splineControllerFactory) :
    m_SelectionController(ICurveEditorSelectionControllerPrivate::Create()),
    m_SplineControllerFactory(splineControllerFactory)
{
    EDITOR_ASSERT(m_SelectionController);
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
    NotifySplineControllerCreated(splineController);

    return true;
}

bool CCurveEditorController::DestroySpline(const ICurveEditorSplineDataModelSharedPtr& splineDataModel)
{
    const auto iterator = m_SplineControllers.find(splineDataModel);
    if (iterator == m_SplineControllers.end())
        return false;

    NotifySplineControllerDestroyed(iterator->second);
    m_SplineControllers.erase(iterator);

    return true;
}

void CCurveEditorController::SetSelectionDataModel(const ICurveEditorSelectionDataModelSharedPtr& selectionDataModel)
{
    EDITOR_ASSERT(m_SelectionController);
    if (!m_SelectionController)
        return;

    const auto result = m_SelectionController->SetDataModel(selectionDataModel);
    EDITOR_ASSERT(result);
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

void CCurveEditorController::OnDataModelChanged()
{
    RecreateSplineControllers();
}

void CCurveEditorController::RecreateSplineControllers()
{
    VisitObjectsMap(m_SplineControllers, [this](const auto& splineController)
    {
        NotifySplineControllerDestroyed(splineController);
    });

    m_SplineControllers.clear();

    const auto& dataModel = GetDataModel();
    if (!dataModel)
        return;

    if (m_SelectionController)
        m_SelectionController->SetDataModel(dataModel->GetSelectionDataModel());

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

ICurveEditorSelectionControllerSharedPtr CCurveEditorController::GetSelectionController() const noexcept
{
    return m_SelectionController;
}

IEditorListenerUniquePtr CCurveEditorController::CreateListener()
{
    return std::make_unique<CCurveEditorDataModelControllerListener>(*this);
}

void CCurveEditorController::NotifySplineControllerCreated(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    EDITOR_ASSERT(splineController);
    if (!splineController)
        return;

    if (m_SelectionController)
    {
        const auto result = m_SelectionController->RegisterSpline(splineController);
        EDITOR_ASSERT(result);
    }

    NotifyListeners(&ICurveEditorControllerListener::OnSplineCreated, splineController);
}

void CCurveEditorController::NotifySplineControllerDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    EDITOR_ASSERT(splineController);
    if (!splineController)
        return;

    if (m_SelectionController)
    {
        const auto result = m_SelectionController->UnregisterSpline(splineController);
        EDITOR_ASSERT(result);
    }

    NotifyListeners(&ICurveEditorControllerListener::OnSplineDestroyed, splineController);
}

ICurveEditorControllerUniquePtr ICurveEditorController::Create(ICurveEditorSplineControllerFactory& splineControllerFactory)
{
    return std::make_unique<CCurveEditorController>(splineControllerFactory);
}