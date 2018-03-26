#pragma  once
#if !defined(__EDITOR_LISTENER_H__)

using EditorListenerHandle = size_t;
const EditorListenerHandle InvalidListenerHandle = 0;

class IEditorListener
{
public:
    virtual ~IEditorListener() = default;
};

#endif //__EDITOR_LISTENER_H__