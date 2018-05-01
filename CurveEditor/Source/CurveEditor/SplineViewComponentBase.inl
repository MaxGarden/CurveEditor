template<typename SuperClass>
CCurveEditorSplineViewComponentBase<SuperClass>::CCurveEditorSplineViewComponentBase(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

template<typename SuperClass>
void CCurveEditorSplineViewComponentBase<SuperClass>::OnFrame()
{
    const auto drawList = ImGui::GetWindowDrawList();
    EDITOR_ASSERT(drawList);
    if (!drawList)
        return;

    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return;

    OnFrame(*drawList, *controller);
}

template<typename SuperClass>
void CCurveEditorSplineViewComponentBase<SuperClass>::OnFrame(ImDrawList&, ICurveEditorSplineController&)
{
    //to override
}

template<typename SuperClass>
ICurveEditorView& CCurveEditorSplineViewComponentBase<SuperClass>::GetEditorView() const noexcept
{
    return m_EditorView;
}

template<typename SplineComponentType>
CCurveEditorSplineComponentBorderRenderableBase<SplineComponentType>::CCurveEditorSplineComponentBorderRenderableBase(SplineComponentConstWeakPtr && splineComponentView, ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) :
    m_SplineComponentView(std::move(splineComponentView)),
    m_BorderStyleColor(borderStyleColor),
    m_ThicknessStyle(thicknessStyle)
{
    EDITOR_ASSERT(!m_SplineComponentView.expired());
}

template<typename SplineComponentType>
const typename CCurveEditorSplineComponentBorderRenderableBase<SplineComponentType>::SplineComponentConstWeakPtr& CCurveEditorSplineComponentBorderRenderableBase<SplineComponentType>::GetSplineComponentView() const noexcept
{
    return m_SplineComponentView;
}


template<typename SplineComponentType>
ECurveEditorStyleColor CCurveEditorSplineComponentBorderRenderableBase<SplineComponentType>::GetBorderStyleColor() const noexcept
{
    return m_BorderStyleColor;
}

template<typename SplineComponentType>
ECurveEditorStyleFloat CCurveEditorSplineComponentBorderRenderableBase<SplineComponentType>::GetThichnessStyle() const noexcept
{
    return m_ThicknessStyle;
}