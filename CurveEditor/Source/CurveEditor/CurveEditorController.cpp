#include "pch.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"
#include "SplineDataModel.h"

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

bool CCurveEditorController::CreateSpline(std::string&& name)
{
	if (!m_DataModel)
		return false;

	const auto splineDataModel = std::make_shared<CCurveEditorSplineDataModel>(std::move(name));
	
	if (!m_DataModel->AddSplineDataModel(splineDataModel))
		return false;

	NotifyProtocols(&ICurveEditorProtocol::OnSplineDataModelCreated, splineDataModel);
	return false;
}

bool CCurveEditorController::DestroySpline(const std::string& name)
{
	if (!m_DataModel)
		return false;

	const auto& splineDataModels = m_DataModel->GetSplinesDataModels();
	const auto iterator = std::find_if(splineDataModels.begin(), splineDataModels.end(), [&name](const auto& spline)
	{
		if (!spline)
			return false;

		return name.compare(spline->GetName()) == 0;
	});

	if (iterator == splineDataModels.end())
		return nullptr;

	const auto splineDataModel = *iterator;
	if (!splineDataModel)
		return false;

	if (!m_DataModel->RemoveSplineDataModel(splineDataModel))
		return false;

	NotifyProtocols(&ICurveEditorProtocol::OnSplineDataModelDestroyed, splineDataModel);
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