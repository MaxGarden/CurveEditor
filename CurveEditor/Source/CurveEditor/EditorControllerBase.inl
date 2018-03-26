template<typename SuperClass, typename DataModelType, typename ListenerType>
bool CEditorControllerBase<SuperClass, DataModelType, ListenerType>::SetDataModel(const IEditorDataModelSharedPtr& dataModel)
{
    if (!dataModel)
    {
        m_DataModel.reset();
        OnDataModelChanged();
        return true;
    }

    const auto curveEditorDataModel = std::dynamic_pointer_cast<DataModelType>(dataModel);
    if (!curveEditorDataModel)
        return false;

    m_DataModel = std::move(curveEditorDataModel);
    OnDataModelChanged();

    return true;
}

template<typename SuperClass, typename DataModelType, typename ListenerType>
std::optional<EditorListenerHandle> CEditorControllerBase<SuperClass, DataModelType, ListenerType>::RegisterListener(IEditorListenerUniquePtr&& listener)
{
    const auto curveEditorListener = dynamic_cast<ListenerType*>(listener.get());
    if (!curveEditorListener)
        return std::nullopt;

    m_Listeners.emplace_back(std::unique_ptr<ListenerType>(curveEditorListener));
    return reinterpret_cast<EditorListenerHandle>(listener.release());
}

template<typename SuperClass, typename DataModelType, typename ListenerType>
bool CEditorControllerBase<SuperClass, DataModelType, ListenerType>::UnregisterListener(const EditorListenerHandle& handle)
{
    const auto iterator = std::remove_if(m_Listeners.begin(), m_Listeners.end(), [&handle](const auto& listener)
    {
        return reinterpret_cast<EditorListenerHandle>(listener.get()) == handle;
    });

    if (iterator == m_Listeners.end())
        return false;

    m_Listeners.erase(iterator, m_Listeners.end());
    return true;
}

template<typename SuperClass, typename DataModelType, typename ListenerType>
template<typename ListenerMethod, typename... Arguments>
void CEditorControllerBase<SuperClass, DataModelType, ListenerType>::NotifyListeners(ListenerMethod method, Arguments&&... arguments) const
{
    for (const auto& listener : m_Listeners)
    {
        if (listener)
            (listener.get()->*method)(arguments...);
    }
}

template<typename SuperClass, typename DataModelType, typename ListenerType>
void CEditorControllerBase<SuperClass, DataModelType, ListenerType>::OnDataModelChanged()
{
    //to override
}

template<typename SuperClass, typename DataModelType, typename ListenerType>
const std::shared_ptr<DataModelType>& CEditorControllerBase<SuperClass, DataModelType, ListenerType>::GetDataModel() const noexcept
{
    return m_DataModel;
}