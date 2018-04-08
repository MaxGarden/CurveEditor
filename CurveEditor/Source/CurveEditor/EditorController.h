#pragma  once
#if !defined(__EDITOR_CONTROLLER_H__)

#include "EditorListener.h"

class IEditorController : public IEditorListenable
{
public:
    virtual ~IEditorController() = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) = 0;
};

#endif //__EDITOR_CONTROLLER_H__