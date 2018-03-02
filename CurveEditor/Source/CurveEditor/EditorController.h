#pragma  once
#if !defined(__EDITOR_CONTROLLER_H__)

#include <memory>

class IEditorController
{
public:
    virtual ~IEditorController() = default;

    virtual void OnFrame() = 0;
};

#endif //__EDITOR_CONTROLLER_H__