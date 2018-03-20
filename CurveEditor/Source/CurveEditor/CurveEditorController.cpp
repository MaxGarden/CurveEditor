#include "pch.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"
#include "SplineDataModel.h"
#include "SplineController.h"

CCurveEditorController::CCurveEditorController(ICurveEditorSplineControllerFactory& splineControllerFactory) :
    m_SplineControllerFactory(splineControllerFactory)
{
}

bool CCurveEditorController::SetDataModel(const IEditorDataModelSharedPtr& dataModel) noexcept
{
    if (!dataModel)
    {
        m_DataModel.reset();
        return true;
    }

    const auto curveEditorDataModel = std::dynamic_pointer_cast<CCurveEditorDataModel>(dataModel);
    if (!curveEditorDataModel)
        return false;

    m_DataModel = std::move(curveEditorDataModel);
    return true;
}

bool CCurveEditorController::CreateSpline(std::string name)
{
    if (!m_DataModel)
        return false;

    const auto splineDataModel = ICurveEditorSplineDataModelSharedPtr(ICurveEditorSplineDataModel::Create(std::move(name)));

    if (!m_DataModel->AddSplineDataModel(splineDataModel))
        return false;

    const auto splineController = ICurveEditorSplineControllerSharedPtr(m_SplineControllerFactory.Create(splineDataModel));

    const auto cleanup = [this, &splineDataModel]()
    {
        m_DataModel->RemoveSplineDataModel(splineDataModel);
        return false;
    };

    if (!splineController)
        return cleanup();

    auto isValid = true;
    isValid &= splineController->SetDataModel(splineDataModel);

    if (!isValid)
        return cleanup();

    m_SplineControllers.emplace(splineDataModel, splineController);

    NotifyProtocols(&ICurveEditorProtocol::OnSplineCreated, splineController);
    return false;
}

bool CCurveEditorController::DestroySpline(const std::string& name)
{
    if (!m_DataModel)
        return false;

    const auto& splineDataModels = m_DataModel->GetSplinesDataModels();
    const auto dataModelIterator = std::find_if(splineDataModels.begin(), splineDataModels.end(), [&name](const auto& spline)
    {
        if (!spline)
            return false;

        return name.compare(spline->GetName()) == 0;
    });

    if (dataModelIterator == splineDataModels.end())
        return nullptr;

    const auto splineDataModel = *dataModelIterator;
    if (!splineDataModel)
        return false;

    if (!m_DataModel->RemoveSplineDataModel(splineDataModel))
        return false;

    const auto controllerIterator = m_SplineControllers.find(splineDataModel);
    EDITOR_ASSERT(controllerIterator != m_SplineControllers.end());
    if (controllerIterator != m_SplineControllers.end())
    {
        NotifyProtocols(&ICurveEditorProtocol::OnSplineDestroyed, controllerIterator->second);
        m_SplineControllers.erase(controllerIterator);
    }

    return false;
}

std::optional<CurveEditorProtocolHandle> CCurveEditorController::RegisterProtocol(ICurveEditorProtocolUniquePtr&& protocol)
{
    if (!protocol)
        return std::nullopt;

    m_Protocols.emplace_back(std::move(protocol));
    return reinterpret_cast<CurveEditorProtocolHandle>(m_Protocols.back().get());
}

bool CCurveEditorController::UnregisterProtocol(const CurveEditorProtocolHandle& handle)
{
    const auto iterator = std::remove_if(m_Protocols.begin(), m_Protocols.end(), [&handle](const auto& protocol)
    {
        return reinterpret_cast<CurveEditorProtocolHandle>(protocol.get()) == handle;
    });

    if (iterator == m_Protocols.end())
        return false;

    m_Protocols.erase(iterator, m_Protocols.end());
    return true;
}

const SEditorStyle& CCurveEditorController::GetEditorStyle() const noexcept
{
    static SEditorStyle nullStyle;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetStyle();

    return nullStyle;
}

const CCurveEditorDataModelSharedPtr& CCurveEditorController::GetDataModel() const noexcept
{
    return m_DataModel;
}

void CCurveEditorController::VisitSplineControllers(const std::function<void(const ICurveEditorSplineControllerSharedPtr&)>& visitor) const noexcept
{
    if (!visitor)
        return;

    for (const auto& controllerPair : m_SplineControllers)
    {
        if (const auto& controller = controllerPair.second)
            visitor(controller);
    }
}