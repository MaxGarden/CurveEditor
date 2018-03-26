#pragma  once
#if !defined(__EDITOR_CONTROLLER_H__)

#include "EditorListener.h"

class IEditorController
{
public:
    virtual ~IEditorController() = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) = 0;

    virtual std::optional<EditorListenerHandle> RegisterListener(IEditorListenerUniquePtr&& listener) = 0;
    virtual bool UnregisterListener(const EditorListenerHandle& handle) = 0;
};

#endif //__EDITOR_CONTROLLER_H__