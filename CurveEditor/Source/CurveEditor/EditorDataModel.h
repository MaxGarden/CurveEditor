#pragma  once
#if !defined(__EDITOR_DATA_MODEL_H__)

#include "EditorListener.h"

class IEditorDataModel : public IEditorListenable
{
public:
    virtual ~IEditorDataModel() = default;
};

DECLARE_POINTERS(IEditorDataModel);

#endif //__EDITOR_DATA_MODEL_H__