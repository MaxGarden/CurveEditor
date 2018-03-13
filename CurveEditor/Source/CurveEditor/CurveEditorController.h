#pragma  once
#if !defined(__CURVE_EDITOR_CONTROLLER_H__)

#include "EditorController.h"
#include "Style.h"

class ICurveEditorProtocol
{
public:
    virtual ~ICurveEditorProtocol() = default;

    virtual void OnSplineDataModelCreated(const CCurveEditorSplineDataModelSharedPtr& splineDataModel) = 0;
    virtual void OnSplineDataModelDestroyed(const CCurveEditorSplineDataModelSharedPtr& splineDataModel) = 0;
};
using CurveEditorProtocolHandle = size_t;

class CCurveEditorController final : public IEditorController
{
public:
    CCurveEditorController() = default;
    virtual ~CCurveEditorController() override final = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) noexcept override final;

    bool CreateSpline(std::string&& name);
    bool DestroySpline(const std::string& name);

    std::optional<CurveEditorProtocolHandle> RegisterProtocol(ICurveEditorProtocolUniquePtr&& protocol);
	bool UnregisterProtocol(const CurveEditorProtocolHandle& handle);

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
};

#endif //__CURVE_EDITOR_CONTROLLER_H__