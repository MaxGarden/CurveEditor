#pragma once
#if !defined(__EDITOR_LISTENABLE_BASE_H__)

template<typename SuperClass, typename ListenerType>
class CEditorListenableBase : public SuperClass
{
public:
    CEditorListenableBase() = default;
    virtual ~CEditorListenableBase() override = default;

    virtual std::optional<EditorListenerHandle> RegisterListener(IEditorListenerUniquePtr&& listener) override;
    virtual bool UnregisterListener(const EditorListenerHandle& handle) override;

protected:
    template<typename ListenerMethod, typename... Arguments>
    void NotifyListeners(ListenerMethod method, Arguments&&... arguments);

private:
    std::vector<std::unique_ptr<ListenerType>> m_Listeners;
};

#include "EditorListenableBase.inl"

#endif //__EDITOR_LISTENABLE_BASE_H__