#pragma  once
#if !defined(__EDITOR_CONTROLLER_BASE_H__)

#include "EditorController.h"

template<typename SuperClass, typename DataModelType, typename ProtocolType>
class CEditorControllerBase : public SuperClass
{
public:
    CEditorControllerBase() = default;
    virtual ~CEditorControllerBase() = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) override;

    virtual std::optional<EditorProtocolHandle> RegisterProtocol(IEditorProtocolUniquePtr&& protocol) override;
    virtual bool UnregisterProtocol(const EditorProtocolHandle& handle) override;

protected:
    template<typename ProtocolMethod, typename... Arguments>
    void NotifyProtocols(ProtocolMethod method, Arguments&&... arguments) const;

    virtual void OnDataModelChanged();

    const std::shared_ptr<DataModelType>& GetDataModel() const noexcept;

    using Super = CEditorControllerBase<SuperClass, DataModelType, ProtocolType>;

private:
    std::shared_ptr<DataModelType> m_DataModel;
    std::vector<std::unique_ptr<ProtocolType>> m_Protocols;
};

#include "EditorControllerBase.inl"

#endif //__EDITOR_CONTROLLER_BASE_H__