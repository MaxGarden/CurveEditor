#pragma  once
#if !defined(__EDITOR_CONTROLLER_H__)

#include <memory>

class IEditorController
{
public:
    virtual ~IEditorController() = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) noexcept = 0;
};

#endif //__EDITOR_CONTROLLER_H__