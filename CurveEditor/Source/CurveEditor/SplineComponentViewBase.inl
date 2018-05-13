template<typename SuperClass, typename ControllerType, ECurveEditorSplineComponentType SplineComponentType>
CCurveEditorSplineComponentViewBase<SuperClass, ControllerType, SplineComponentType>::CCurveEditorSplineComponentViewBase(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

template<typename SuperClass, typename ControllerType, ECurveEditorSplineComponentType SplineComponentType>
void CCurveEditorSplineComponentViewBase<SuperClass, ControllerType, SplineComponentType>::OnFrame()
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

template<typename SuperClass, typename ControllerType, ECurveEditorSplineComponentType SplineComponentType>
ECurveEditorSplineComponentType CCurveEditorSplineComponentViewBase<SuperClass, ControllerType, SplineComponentType>::GetType() const noexcept
{
    return SplineComponentType;
}

template<typename SuperClass, typename ControllerType, ECurveEditorSplineComponentType SplineComponentType>
const SplineID& CCurveEditorSplineComponentViewBase<SuperClass, ControllerType, SplineComponentType>::GetSplineID() const noexcept
{
    if (const auto& controller = GetController())
        return controller->GetSplineID();

    return ICurveEditorSplineDataModel::InvalidSplineID();
}

template<typename SuperClass, typename ControllerType, ECurveEditorSplineComponentType SplineComponentType>
std::optional<size_t> CCurveEditorSplineComponentViewBase<SuperClass, ControllerType, SplineComponentType>::GetIndex() const noexcept
{
    if (const auto& controller = GetController())
        return controller->GetIndex();

    return std::nullopt;
}


template<typename SuperClass, typename ControllerType, ECurveEditorSplineComponentType SplineComponentType>
void CCurveEditorSplineComponentViewBase<SuperClass, ControllerType, SplineComponentType>::OnFrame(ImDrawList&, ControllerType&)
{
    //to override
}

template<typename SuperClass, typename ControllerType, ECurveEditorSplineComponentType SplineComponentType>
ICurveEditorView& CCurveEditorSplineComponentViewBase<SuperClass, ControllerType, SplineComponentType>::GetEditorView() const noexcept
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