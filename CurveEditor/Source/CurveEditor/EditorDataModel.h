#pragma  once
#if !defined(__EDITOR_DATA_MODEL_H__)

class IEditorDataModel
{
public:
    virtual ~IEditorDataModel() = default;
};

DECLARE_POINTERS(IEditorDataModel);

#endif //__EDITOR_DATA_MODEL_H__