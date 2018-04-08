#pragma  once
#if !defined(__EDITOR_LISTENER_H__)

using EditorListenerHandle = size_t;
const EditorListenerHandle InvalidListenerHandle = 0;

class IEditorListener
{
public:
    virtual ~IEditorListener() = default;
};

class IEditorListenable
{
public:
    virtual ~IEditorListenable() = default;

    virtual std::optional<EditorListenerHandle> RegisterListener(IEditorListenerUniquePtr&& listener) = 0;
    virtual bool UnregisterListener(const EditorListenerHandle& handle) = 0;
};

#endif //__EDITOR_LISTENER_H__