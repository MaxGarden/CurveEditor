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

    virtual void VisitSplineControllers(const ConstVisitorType<ICurveEditorSplineControllerSharedPtr>& visitor) const noexcept override final;

    virtual const SCurveEditorStyle& GetEditorStyle() const noexcept override final;

private:
    virtual void OnDataModelChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

    void RecreateSplineControllers();

private:
    std::map<ICurveEditorSplineDataModelConstSharedPtr, ICurveEditorSplineControllerSharedPtr> m_SplineControllers;
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
    const auto& dataModel = GetDataModel();
    if (!dataModel)
        return;

    //TODO
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