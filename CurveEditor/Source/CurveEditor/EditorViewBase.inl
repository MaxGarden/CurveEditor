template<typename SuperClass, typename ControllerType>
void CEditorViewBase<SuperClass, ControllerType>::OnControllerChanged()
{
    //to override
}

template<typename SuperClass, typename ControllerType>
const std::shared_ptr<ControllerType>& CEditorViewBase<SuperClass, ControllerType>::GetController() const noexcept
{
    return m_Controller;
}

template<typename SuperClass, typename ControllerType>
bool CEditorViewBase<SuperClass, ControllerType>::SetController(const IEditorControllerSharedPtr& controller) noexcept
{
    if (!controller)
    {
        m_Controller.reset();
        OnControllerChanged();
        return true;
    }

    const auto curveEditorController = std::dynamic_pointer_cast<ControllerType>(controller);
    if (!curveEditorController)
        return false;

    m_Controller = std::move(curveEditorController);
    OnControllerChanged();

    return true;
}

template<typename SuperClass, typename ControllerType>
void CEditorViewBase<SuperClass, ControllerType>::OnFrame()
{
    //to override
}