template<typename SuperClass, ECurveEditorSplineComponentType SplineComponentType>
CCurveEditorSplineComponentViewBase<SuperClass, SplineComponentType>::CCurveEditorSplineComponentViewBase(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

template<typename SuperClass, ECurveEditorSplineComponentType SplineComponentType>
void CCurveEditorSplineComponentViewBase<SuperClass, SplineComponentType>::OnFrame()
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

template<typename SuperClass, ECurveEditorSplineComponentType SplineComponentType>
const SplineID& CCurveEditorSplineComponentViewBase<SuperClass, SplineComponentType>::GetSplineID() const noexcept
{
    const auto controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return ICurveEditorSplineDataModel::InvalidSplineID();

    return controller->GetID();
}

template<typename SuperClass, ECurveEditorSplineComponentType SplineComponentType>
ECurveEditorSplineComponentType CCurveEditorSplineComponentViewBase<SuperClass, SplineComponentType>::GetType() const noexcept
{
    return SplineComponentType;
}

template<typename SuperClass, ECurveEditorSplineComponentType SplineComponentType>
void CCurveEditorSplineComponentViewBase<SuperClass, SplineComponentType>::OnFrame(ImDrawList&, ICurveEditorSplineController&)
{
    //to override
}

template<typename SuperClass, ECurveEditorSplineComponentType SplineComponentType>
ICurveEditorView& CCurveEditorSplineComponentViewBase<SuperClass, SplineComponentType>::GetEditorView() const noexcept
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