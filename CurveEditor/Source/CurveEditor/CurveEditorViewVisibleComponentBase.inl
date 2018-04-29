template<typename SuperClass, typename ControllerType>
CCurveEditorViewVisibleComponentBase<SuperClass, ControllerType>::CCurveEditorViewVisibleComponentBase(ICurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView)
{
}

template<typename SuperClass, typename ControllerType>
void CCurveEditorViewVisibleComponentBase<SuperClass, ControllerType>::OnFrame()
{
    EDITOR_ASSERT(ImGui::GetCurrentContext());
    const auto drawList = ImGui::GetWindowDrawList();
    if (!drawList)
        return;

    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return;

    OnFrame(*drawList, *controller);
}

template<typename SuperClass, typename ControllerType>
void CCurveEditorViewVisibleComponentBase<SuperClass, ControllerType>::OnFrame(ImDrawList&, ICurveEditorController&)
{
    //to override
}