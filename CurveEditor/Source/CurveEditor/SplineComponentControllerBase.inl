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
bool CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>::SetControlPoints(const SplineControlPointsPositions& positions)
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    return dataModel->SetControlPoints(positions);
}

template<typename SuperClass, ECurveEditorSplineComponentType ComponentType>
std::optional<ax::pointf> CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>::GetControlPoint(size_t controlPointIndex) const noexcept
{
    const auto& controlPoints = GetControlPoints();
    EDITOR_ASSERT(controlPointIndex < controlPoints.size());
    if (controlPointIndex  >= controlPoints.size())
        return std::nullopt;

    return controlPoints[controlPointIndex];
}

template<typename SuperClass, ECurveEditorSplineComponentType ComponentType>
bool CCurveEditorSplineComponentControllerBase<SuperClass, ComponentType>::MoveControlPoints(SplineControlPointsPositions deltaPositions)
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    const auto& controlPoints = dataModel->GetControlPoints();
    const auto lastControlPointIndex = deltaPositions.rbegin()->ControlPointIndex;

    EDITOR_ASSERT(lastControlPointIndex < controlPoints.size());
    if (lastControlPointIndex >= controlPoints.size())
        return false;

    VisitObjectsContainerInterruptible(deltaPositions, [&controlPoints](auto& singlePosition)
    {
        singlePosition.Position += controlPoints[singlePosition.ControlPointIndex];
        return true;
    });

    return dataModel->SetControlPoints(deltaPositions);
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