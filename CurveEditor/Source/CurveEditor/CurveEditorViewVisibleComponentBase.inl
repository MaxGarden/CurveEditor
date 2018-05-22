template<typename SuperClass /*= ICurveEditorViewComponent*/, typename ControllerType /*= ICurveEditorController*/>
CCurveEditorViewVisibleComponentBase<SuperClass, ControllerType>::CCurveEditorViewVisibleComponentBase(ICurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView)
{
}

template<typename SuperClass /*= ICurveEditorViewComponent*/, typename ControllerType /*= ICurveEditorController*/>
void CCurveEditorViewVisibleComponentBase<SuperClass, ControllerType>::OnFrame(ImDrawList& drawList)
{
    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return;

    OnFrame(drawList, *controller);
}

template<typename SuperClass /*= ICurveEditorViewComponent*/, typename ControllerType /*= ICurveEditorController*/>
void CCurveEditorViewVisibleComponentBase<SuperClass, ControllerType>::OnFrame()
{
    EDITOR_ASSERT(ImGui::GetCurrentContext());
    const auto drawList = ImGui::GetWindowDrawList();
    if (!drawList)
        return;

    OnFrame(*drawList);
}

template<typename SuperClass /*= ICurveEditorViewComponent*/, typename ControllerType /*= ICurveEditorController*/>
void CCurveEditorViewVisibleComponentBase<SuperClass, ControllerType>::OnFrame(ImDrawList&, ControllerType&)
{
    //to override
}