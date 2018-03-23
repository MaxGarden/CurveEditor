template<typename SuperClass, typename DataModelType, typename ProtocolType>
bool CEditorControllerBase<SuperClass, DataModelType, ProtocolType>::SetDataModel(const IEditorDataModelSharedPtr& dataModel)
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

template<typename SuperClass, typename DataModelType, typename ProtocolType>
std::optional<EditorProtocolHandle> CEditorControllerBase<SuperClass, DataModelType, ProtocolType>::RegisterProtocol(IEditorProtocolUniquePtr&& protocol)
{
    const auto curveEditorProtocol = dynamic_cast<ProtocolType*>(protocol.get());
    if (!curveEditorProtocol)
        return std::nullopt;

    m_Protocols.emplace_back(std::unique_ptr<ProtocolType>(curveEditorProtocol));
    return reinterpret_cast<EditorProtocolHandle>(protocol.release());
}

template<typename SuperClass, typename DataModelType, typename ProtocolType>
bool CEditorControllerBase<SuperClass, DataModelType, ProtocolType>::UnregisterProtocol(const EditorProtocolHandle& handle)
{
    const auto iterator = std::remove_if(m_Protocols.begin(), m_Protocols.end(), [&handle](const auto& protocol)
    {
        return reinterpret_cast<EditorProtocolHandle>(protocol.get()) == handle;
    });

    if (iterator == m_Protocols.end())
        return false;

    m_Protocols.erase(iterator, m_Protocols.end());
    return true;
}

template<typename SuperClass, typename DataModelType, typename ProtocolType>
template<typename ProtocolMethod, typename... Arguments>
void CEditorControllerBase<SuperClass, DataModelType, ProtocolType>::NotifyProtocols(ProtocolMethod method, Arguments&&... arguments) const
{
    for (const auto& protocol : m_Protocols)
    {
        if (protocol)
            (protocol.get()->*method)(arguments...);
    }
}

template<typename SuperClass, typename DataModelType, typename ProtocolType>
void CEditorControllerBase<SuperClass, DataModelType, ProtocolType>::OnDataModelChanged()
{
    //to override
}

template<typename SuperClass, typename DataModelType, typename ProtocolType>
const std::shared_ptr<DataModelType>& CEditorControllerBase<SuperClass, DataModelType, ProtocolType>::GetDataModel() const noexcept
{
    return m_DataModel;
}