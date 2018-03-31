#include "pch.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"
#include "SplineDataModel.h"
#include "SplineController.h"

CCurveEditorController::CCurveEditorController(ICurveEditorSplineControllerFactory& splineControllerFactory) :
    m_SplineControllerFactory(splineControllerFactory)
{
}

std::optional<CCurveEditorController::SplineHandle> CCurveEditorController::CreateSpline(std::string_view name, unsigned int color)
{
    const auto& dataModel = GetDataModel();
    if (!dataModel)
        return std::nullopt;

    const auto splineDataModel = dataModel->AddSplineDataModel(name, color);

    if (!splineDataModel)
        return std::nullopt;

    const auto splineController = ICurveEditorSplineControllerSharedPtr(m_SplineControllerFactory.Create(splineDataModel));

    const auto cleanup = [&dataModel, &splineDataModel]()
    {
        dataModel->RemoveSplineDataModel(splineDataModel);
        return std::nullopt;
    };

    if (!splineController)
        return cleanup();

    auto isValid = true;
    isValid &= splineController->SetDataModel(splineDataModel);

    if (!isValid)
        return cleanup();

    const auto splineHandle = reinterpret_cast<SplineHandle>(splineController.get());

    m_SplineStorages.emplace(splineHandle, SplineStorage{ splineDataModel, splineController });

    NotifyListeners(&ICurveEditorListener::OnSplineCreated, splineController);
    return splineHandle;
}

bool CCurveEditorController::DestroySpline(const SplineHandle& handle)
{
    const auto& dataModel = GetDataModel();
    if (!dataModel)
        return false;

    const auto iterator = m_SplineStorages.find(handle);
    if (iterator == m_SplineStorages.end())
        return false;

    const auto& storage = iterator->second;

    if (!dataModel->RemoveSplineDataModel(storage.m_DataModel))
    {
        EDITOR_ASSERT(false && "Cannot remove data model");
        return false;
    }

    NotifyListeners(&ICurveEditorListener::OnSplineDestroyed, storage.m_Controller);
    m_SplineStorages.erase(iterator);

    return true;
}

const SCurveEditorStyle& CCurveEditorController::GetEditorStyle() const noexcept
{
    static SCurveEditorStyle nullStyle;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetStyle();

    return nullStyle;
}

void CCurveEditorController::VisitSplineControllers(const std::function<void(const ICurveEditorSplineControllerSharedPtr&)>& visitor) const noexcept
{
    if (!visitor)
        return;

    for (const auto& controllerPair : m_SplineStorages)
    {
        if (const auto& controller = controllerPair.second.m_Controller)
            visitor(controller);
    }
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