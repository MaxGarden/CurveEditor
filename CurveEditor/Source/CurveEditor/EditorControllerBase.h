#pragma  once
#if !defined(__EDITOR_CONTROLLER_BASE_H__)

#include "EditorController.h"

template<typename SuperClass, typename DataModelType, typename ListenerType>
class CEditorControllerBase : public SuperClass
{
public:
    CEditorControllerBase() = default;
    virtual ~CEditorControllerBase() = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) override;

    virtual std::optional<EditorListenerHandle> RegisterListener(IEditorListenerUniquePtr&& listener) override;
    virtual bool UnregisterListener(const EditorListenerHandle& handle) override;

protected:
    template<typename ListenerMethod, typename... Arguments>
    void NotifyListeners(ListenerMethod method, Arguments&&... arguments) const;

    virtual void OnDataModelChanged();

    const std::shared_ptr<DataModelType>& GetDataModel() const noexcept;

    using Super = CEditorControllerBase<SuperClass, DataModelType, ListenerType>;

private:
    std::shared_ptr<DataModelType> m_DataModel;
    std::vector<std::unique_ptr<ListenerType>> m_Listeners;
};

#include "EditorControllerBase.inl"

#endif //__EDITOR_CONTROLLER_BASE_H__