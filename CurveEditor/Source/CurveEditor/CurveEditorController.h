#pragma  once
#if !defined(__CURVE_EDITOR_CONTROLLER_H__)

#include "EditorController.h"
#include "Style.h"

class ICurveEditorProtocol
{
public:
    virtual ~ICurveEditorProtocol() = default;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
};
using CurveEditorProtocolHandle = size_t;

class CCurveEditorController final : public IEditorController
{
public:
    CCurveEditorController(ICurveEditorSplineControllerFactory& splineControllerFactory);
    virtual ~CCurveEditorController() override final = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) noexcept override final;

    bool CreateSpline(std::string name);
    bool DestroySpline(const std::string& name);

    std::optional<CurveEditorProtocolHandle> RegisterProtocol(ICurveEditorProtocolUniquePtr&& protocol);
	bool UnregisterProtocol(const CurveEditorProtocolHandle& handle);

    void VisitSplineControllers(const std::function<void(const ICurveEditorSplineControllerSharedPtr&)>& visitor) const noexcept;

	const SEditorStyle& GetEditorStyle() const noexcept;

private:
	template<typename Method, typename... Arguments>
	void NotifyProtocols(Method method, Arguments&&... arguments) const
	{
		for (const auto& protocol : m_Protocols)
		{
			if (protocol)
				(protocol.get()->*method)(arguments...);
		}
	}

	const CCurveEditorDataModelSharedPtr& GetDataModel() const noexcept;

private:
    CCurveEditorDataModelSharedPtr m_DataModel;
    std::vector<ICurveEditorProtocolUniquePtr> m_Protocols;
    std::map<ICurveEditorSplineDataModelConstSharedPtr, ICurveEditorSplineControllerSharedPtr> m_SplineControllers;
    ICurveEditorSplineControllerFactory& m_SplineControllerFactory;
};

#endif //__CURVE_EDITOR_CONTROLLER_H__