template<typename SuperClass, ECurveEditorSplineComponentType ComponentType>
const SplineID& CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>::GetSplineID() const noexcept
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return ICurveEditorSplineDataModel::InvalidSplineID();

    return dataModel->GetID();
}

template<typename SuperClass, ECurveEditorSplineComponentType ComponentType>
ECurveEditorSplineComponentType CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>::GetType() const noexcept
{
    return ComponentType;
}

template<typename SuperClass, ECurveEditorSplineComponentType ComponentType>
bool CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>::SetControlPointPosition(size_t controlPointIndex, const ax::pointf& position)
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    auto& controlPoints = dataModel->GetControlPoints();
    if (controlPointIndex >= controlPoints.size())
        return false;

    controlPoints[controlPointIndex] = position;
    return true;
}

template<typename SuperClass, ECurveEditorSplineComponentType ComponentType>
std::optional<ax::pointf> CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>::GetControlPointPosition(size_t controlPointIndex) const noexcept
{
    const auto& controlPoints = GetControlPoints();
    EDITOR_ASSERT(controlPointIndex < controlPoints.size());
    if (controlPointIndex  >= controlPoints.size())
        return std::nullopt;

    return controlPoints[controlPointIndex];
}

template<typename SuperClass, ECurveEditorSplineComponentType ComponentType>
bool CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>::MoveControlPointPosition(size_t controlPointIndex, const ax::pointf& delta)
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    auto& controlPoints = dataModel->GetControlPoints();
    if (controlPointIndex >= controlPoints.size())
        return false;

    controlPoints[controlPointIndex] += delta;
    return true;
}

static const std::vector<ax::pointf> null;

template<typename SuperClass, ECurveEditorSplineComponentType ComponentType>
const std::vector<ax::pointf>& CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>::GetControlPoints() const noexcept
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return null;

    return dataModel->GetControlPoints();
}