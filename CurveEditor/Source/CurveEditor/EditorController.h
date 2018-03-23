#pragma  once
#if !defined(__EDITOR_CONTROLLER_H__)

#include "EditorProtocol.h"

class IEditorController
{
public:
    virtual ~IEditorController() = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) = 0;

    virtual std::optional<EditorProtocolHandle> RegisterProtocol(IEditorProtocolUniquePtr&& protocol) = 0;
    virtual bool UnregisterProtocol(const EditorProtocolHandle& handle) = 0;
};

#endif //__EDITOR_CONTROLLER_H__