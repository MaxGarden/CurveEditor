#pragma once
#if !defined(__EDITOR_CONTROLLER_BASE_H__)

#include "EditorListenableBase.h"

template<typename SuperClass, typename DataModelType, typename ListenerType>
class CEditorControllerBase : public CEditorListenableBase<SuperClass, ListenerType>
{
public:
    CEditorControllerBase() = default;
    virtual ~CEditorControllerBase() override;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) override;

protected:
    virtual void OnDataModelChanged();
    virtual IEditorListenerUniquePtr CreateListener();

    const std::shared_ptr<DataModelType>& GetDataModel() const noexcept;

    using Super = CEditorControllerBase<SuperClass, DataModelType, ListenerType>;

private:
    std::shared_ptr<DataModelType> m_DataModel;
    EditorListenerHandle m_ListenerHandle;
};

#include "EditorControllerBase.inl"

#endif //__EDITOR_CONTROLLER_BASE_H__