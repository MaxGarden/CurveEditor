template<typename SuperClass, typename ListenerType>
std::optional<EditorListenerHandle> CEditorListenableBase<SuperClass, ListenerType>::RegisterListener(IEditorListenerUniquePtr&& listener)
{
    const auto curveEditorListener = dynamic_cast<ListenerType*>(listener.get());
    if (!curveEditorListener)
        return std::nullopt;

    m_Listeners.emplace_back(std::unique_ptr<ListenerType>(curveEditorListener));
    return reinterpret_cast<EditorListenerHandle>(listener.release());
}

template<typename SuperClass, typename ListenerType>
bool CEditorListenableBase<SuperClass, ListenerType>::UnregisterListener(const EditorListenerHandle& handle)
{
    const auto iterator = std::remove_if(m_Listeners.begin(), m_Listeners.end(), [&handle](const auto& listener)
    {
        return reinterpret_cast<EditorListenerHandle>(listener.get()) == handle;
    });

    if (iterator == m_Listeners.end())
        return false;

    iterator->reset();
    return true;
}

template<typename SuperClass, typename ListenerType>
template<typename ListenerMethod, typename... Arguments>
void CEditorListenableBase<SuperClass, ListenerType>::NotifyListeners(ListenerMethod method, Arguments&&... arguments)
{
    for (const auto& listener : m_Listeners)
    {
        if (listener)
            (listener.get()->*method)(arguments...);
    }

    const auto iterator = std::remove(m_Listeners.begin(), m_Listeners.end(), nullptr);
    m_Listeners.erase(iterator, m_Listeners.end());
}