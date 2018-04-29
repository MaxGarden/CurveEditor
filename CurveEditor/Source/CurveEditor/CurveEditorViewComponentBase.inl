template<typename SuperClass, typename ControllerType>
CCurveEditorViewComponentBase<SuperClass, ControllerType>::CCurveEditorViewComponentBase(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

template<typename SuperClass, typename ControllerType>
bool CCurveEditorViewComponentBase<SuperClass, ControllerType>::Initialize()
{
    //to override
    return true;
}

template<typename SuperClass, typename ControllerType>
void CCurveEditorViewComponentBase<SuperClass, ControllerType>::OnFrame()
{
    //to override
}

template<typename SuperClass, typename ControllerType>
ICurveEditorView& CCurveEditorViewComponentBase<SuperClass, ControllerType>::GetEditorView() const noexcept
{
    return m_EditorView;
}