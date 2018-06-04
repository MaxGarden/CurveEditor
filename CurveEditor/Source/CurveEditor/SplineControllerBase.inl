template<typename SuperClass>
bool CCurveEditorSplineControllerBase<SuperClass>::BeginEditing()
{
    return true;
}

template<typename SuperClass>
bool CCurveEditorSplineControllerBase<SuperClass>::EndEditing()
{
    return true;
}

template<typename SuperClass>
bool CCurveEditorSplineControllerBase<SuperClass>::SaveState()
{
    if (!(m_SavedControlPointsPositions.empty() && m_SavedKnotsControllers.empty() && m_SavedTangentsControllers.empty()))
        return false;

    auto sortResult = true;

    sortResult &= SortComponentsByIndex(m_KnotsControllers);
    sortResult &= SortComponentsByIndex(m_TangentsControllers);

    EDITOR_ASSERT(sortResult);
    if (!sortResult)
        return false;

    const auto& controlPoints = GetControlPoints();
    for (auto i = 0u; i < controlPoints.size(); ++i)
        m_SavedControlPointsPositions.emplace(i, controlPoints[i]);

    m_SavedKnotsControllers = m_KnotsControllers;
    m_SavedTangentsControllers = m_TangentsControllers;

    return true;
}

template<typename SuperClass>
bool CCurveEditorSplineControllerBase<SuperClass>::RestoreState()
{
    if (m_SavedControlPointsPositions.empty() || m_SavedKnotsControllers.empty() || m_SavedTangentsControllers.empty())
        return false;

    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return false;

    m_TangentsControllers = m_SavedTangentsControllers;
    m_KnotsControllers = m_SavedKnotsControllers;

    const auto restoreIndexes = [](const auto& container, const auto setIndexMethod)
    {
        auto index = 0u;
        VisitPointersContainer(container, [&index, &setIndexMethod](auto& controller)
        {
            (controller.*setIndexMethod)(index++);
        });

        EDITOR_ASSERT(index == container.size());
    };

    restoreIndexes(m_KnotsControllers, &ICurveEditorKnotControllerPrivate::SetKnotIndex);
    restoreIndexes(m_TangentsControllers, &ICurveEditorTangentControllerPrivate::SetTangentIndex);

    const auto result = dataModel->SetControlPoints(m_SavedControlPointsPositions);
    EDITOR_ASSERT(result);

    return result;
}

template<typename SuperClass>
void CCurveEditorSplineControllerBase<SuperClass>::ResetSavedState() noexcept
{
    m_SavedControlPointsPositions.clear();
    m_SavedKnotsControllers.clear();
    m_SavedTangentsControllers.clear();
}

template<typename SuperClass>
const SplineID& CCurveEditorSplineControllerBase<SuperClass>::GetID() const noexcept
{
    static const SplineID null = ICurveEditorSplineDataModel::InvalidSplineID();

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetID();

    return null;
}

template<typename SuperClass>
const SplineColor& CCurveEditorSplineControllerBase<SuperClass>::GetColor() const noexcept
{
    static const SplineColor null = {};

    if (const auto dataModel = GetDataModel())
        return dataModel->GetColor();

    return null;
}

template<typename SuperClass>
void CCurveEditorSplineControllerBase<SuperClass>::VisitKnotsControllers(const ConstVisitorType<ICurveEditorKnotControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_KnotsControllers, visitor);
}

template<typename SuperClass>
void CCurveEditorSplineControllerBase<SuperClass>::VisitTangentsControllers(const ConstVisitorType<ICurveEditorTangentControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_TangentsControllers, visitor);
}

template<typename SuperClass>
void CCurveEditorSplineControllerBase<SuperClass>::VisitCurvesControllers(const ConstVisitorType<ICurveEditorCurveControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_CurvesControllers, visitor);
}

template<typename SuperClass>
ICurveEditorSplineComponentControllerSharedPtr CCurveEditorSplineControllerBase<SuperClass>::GetSplineComponentController(ECurveEditorSplineComponentType type, size_t index) const noexcept
{
    const auto findInContainer = [index](const auto& container) -> ICurveEditorSplineComponentControllerSharedPtr
    {
        const auto iterator = std::find_if(container.cbegin(), container.cend(), [index](const auto& splineComponentController)
        {
            return splineComponentController && splineComponentController->GetIndex() == index;
        });

        if (iterator == container.cend())
            return nullptr;

        return *iterator;
    };

    switch (type)
    {
    case ECurveEditorSplineComponentType::Knot:
        return findInContainer(m_KnotsControllers);
    case ECurveEditorSplineComponentType::Curve:
        return findInContainer(m_CurvesControllers);
    case ECurveEditorSplineComponentType::Tangent:
        return findInContainer(m_TangentsControllers);
    default:
        EDITOR_ASSERT(false);
        return nullptr;
    }
}

template<typename SuperClass>
void CCurveEditorSplineControllerBase<SuperClass>::OnControlPointsAdded(const SplineControlPointsPositions& positions)
{
    const auto insertSplineComponentsControllers = [this, &positions](auto& container, const auto demandedSize, const auto setIndexMethod, const auto addControllerMethod)
    {
        std::set<size_t> splineComponentsIndexesToAdd;

        const auto sortResult = SortComponentsByIndex(container);
        EDITOR_ASSERT(sortResult);
        if (!sortResult)
            return;

        auto currentIndex = 0u;

        VisitPointersContainer(container, [&splineComponentsIndexesToAdd, &positions, &setIndexMethod, &currentIndex](auto& splineComponentController) mutable
        {
            while (true)
            {
                const auto result = (splineComponentController.*setIndexMethod)(currentIndex);
                EDITOR_ASSERT(result);

                const auto controlPointIndex = splineComponentController.GetControlPointIndex();
                EDITOR_ASSERT(controlPointIndex);
                if (!controlPointIndex)
                    return;

                if (positions.find(SSplineControlPointSinglePosition(*controlPointIndex, {})) == positions.cend())
                {
                    ++currentIndex;
                    return;
                }
                else
                {
                    splineComponentsIndexesToAdd.emplace(currentIndex);
                    const auto result = (splineComponentController.*setIndexMethod)(++currentIndex);
                    EDITOR_ASSERT(result);
                }
            }
        });

        const auto futureSize = splineComponentsIndexesToAdd.size() + container.size();
        EDITOR_ASSERT(futureSize <= demandedSize);
        if (futureSize > demandedSize)
            return;

        const auto difference = demandedSize - futureSize;
        for (auto i = 0u; i < difference; ++i)
            splineComponentsIndexesToAdd.emplace(currentIndex++);

        VisitObjectsContainer(splineComponentsIndexesToAdd, [this, &addControllerMethod](const auto splineComponentIndex)
        {
            const auto result = (this->*addControllerMethod)(splineComponentIndex);
            EDITOR_ASSERT(result);
        });
    };

    insertSplineComponentsControllers(m_KnotsControllers, CalculateKnotsCount(), &ICurveEditorKnotControllerPrivate::SetKnotIndex, &CCurveEditorSplineControllerBase<SuperClass>::AddKnotController);
    insertSplineComponentsControllers(m_TangentsControllers, CalculateTangentsCount(), &ICurveEditorTangentControllerPrivate::SetTangentIndex, &CCurveEditorSplineControllerBase<SuperClass>::AddTangentController);
    insertSplineComponentsControllers(m_CurvesControllers, CalculateCurvesCount(), &ICurveEditorCurveControllerPrivate::SetCurveIndex, &CCurveEditorSplineControllerBase<SuperClass>::AddCurveController);
}

template<typename SuperClass>
void CCurveEditorSplineControllerBase<SuperClass>::OnControlPointsRemoved(const SplineControlPointsIndexes& indexes)
{
    const auto removeSplineComponentsController = [this, &indexes](const auto& container, const auto demandedSize, const auto setIndexMethod, const auto removeMethod)
    {
        auto splinesComponentsToRemove = decltype(container){};

        VisitObjectsContainer(container, [&splinesComponentsToRemove, &indexes, &setIndexMethod, currentIndex = 0u](const auto& splineComponentController) mutable
        {
            EDITOR_ASSERT(splineComponentController);
            if (!splineComponentController)
                return;

            const auto controlPointIndex = splineComponentController->GetControlPointIndex();
            EDITOR_ASSERT(controlPointIndex);
            if (!controlPointIndex)
                return;

            if (indexes.find(*controlPointIndex) == indexes.cend())
                (splineComponentController.get()->*setIndexMethod)(currentIndex++);
            else
                splinesComponentsToRemove.emplace_back(splineComponentController);
        });

        EDITOR_ASSERT(splinesComponentsToRemove.size() <= container.size());
        if (splinesComponentsToRemove.size() > container.size())
            return;

        const auto futureSize = container.size() - splinesComponentsToRemove.size();
        EDITOR_ASSERT(futureSize >= demandedSize);
        if (futureSize < demandedSize)
            return;

        VisitObjectsContainerInterruptible(container, [&splinesComponentsToRemove, difference = futureSize - demandedSize](const auto& splineComponentController) mutable
        {
            if (difference == 0)
                return false;

            splinesComponentsToRemove.emplace_back(splineComponentController);
            return --difference > 0;
        }, true);

        VisitObjectsContainer(splinesComponentsToRemove, [this, &removeMethod](const auto& splineComponentController)
        {
            const auto result = (this->*removeMethod)(splineComponentController);
            EDITOR_ASSERT(result);
        });
    };

    removeSplineComponentsController(m_KnotsControllers, CalculateKnotsCount(), &ICurveEditorKnotControllerPrivate::SetKnotIndex, &CCurveEditorSplineControllerBase<SuperClass>::RemoveKnotController);
    removeSplineComponentsController(m_TangentsControllers, CalculateTangentsCount(), &ICurveEditorTangentControllerPrivate::SetTangentIndex, &CCurveEditorSplineControllerBase<SuperClass>::RemoveTangentController);
    removeSplineComponentsController(m_CurvesControllers, CalculateCurvesCount(), &ICurveEditorCurveControllerPrivate::SetCurveIndex, &CCurveEditorSplineControllerBase<SuperClass>::RemoveCurveController);
}

template<typename SuperClass>
void CCurveEditorSplineControllerBase<SuperClass>::OnDataModelChanged()
{
    DestroyComponentsControllers();
    CreateComponentsControllers();
}

template<typename SuperClass>
size_t CCurveEditorSplineControllerBase<SuperClass>::CalculateKnotsCount() const noexcept
{
    const auto curvesCount = CalculateCurvesCount();
    if (curvesCount == 0)
        return 0;

    return curvesCount + 1;
}

template<typename SuperClass>
size_t CCurveEditorSplineControllerBase<SuperClass>::CalculateTangentsCount() const noexcept
{
    return CalculateCurvesCount() * 2;
}

template<typename SuperClass>
size_t CCurveEditorSplineControllerBase<SuperClass>::CalculateCurvesCount() const noexcept
{
    const auto controlPointsPerCurve = ControlPointsPerCurve();

    const auto controlPointsCount = GetControlPoints().size();
    if (controlPointsCount < controlPointsPerCurve)
        return 0;

    return 1 + (controlPointsCount - controlPointsPerCurve) / (controlPointsPerCurve - 1);
}

template<typename SuperClass>
template<typename Container, typename GetMethod>
bool CCurveEditorSplineControllerBase<SuperClass>::SortComponentsByPosition(Container& container, GetMethod getPositionMethod) const
{
    auto result = true;

    std::stable_sort(container.begin(), container.end(), [&](const auto& first, const auto& second)
    {
        EDITOR_ASSERT(first && second);
        if (!first || !second)
            return (result = false);

        const auto firstPosition = (first.get()->*getPositionMethod)();
        const auto secondPosition = (second.get()->*getPositionMethod)();

        EDITOR_ASSERT(firstPosition && secondPosition);
        if (!firstPosition || !secondPosition)
            return (result = false);

        return firstPosition->x < secondPosition->x;
    });

    return result;
}

template<typename SuperClass>
template<typename Container>
bool CCurveEditorSplineControllerBase<SuperClass>::SortComponentsByIndex(Container& container) const
{
    auto result = true;

    std::stable_sort(container.begin(), container.end(), [&result](const auto& first, const auto& second)
    {
        if (!first || !second)
            return (result = false);

        const auto firstIndex = first->GetIndex();
        const auto secondIndex = second->GetIndex();
        EDITOR_ASSERT(firstIndex && secondIndex);
        if (!firstIndex || !secondIndex)
            return (result = false);

        return *firstIndex < *secondIndex;
    });

    return result;
}


template<typename SuperClass>
template<typename Container>
void CCurveEditorSplineControllerBase<SuperClass>::UpdateComponents(Container& container) const
{
    for (const auto& componentPair : container)
    {
        const auto result = componentPair.first->SetPosition(componentPair.second);
        EDITOR_ASSERT(result);
    }
}

template<typename SuperClass>
const std::vector<ax::pointf>& CCurveEditorSplineControllerBase<SuperClass>::GetControlPoints() const noexcept
{
    static const std::vector<ax::pointf> null;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetControlPoints();

    return null;
}

template<typename SuperClass>
typename CCurveEditorSplineControllerBase<SuperClass>::KnotsControllers& CCurveEditorSplineControllerBase<SuperClass>::GetKnotsControllers() noexcept
{
    return m_KnotsControllers;
}

template<typename SuperClass>
typename CCurveEditorSplineControllerBase<SuperClass>::TangentsControllers& CCurveEditorSplineControllerBase<SuperClass>::GetTangentsControllers() noexcept
{
    return m_TangentsControllers;
}

template<typename SuperClass>
typename CCurveEditorSplineControllerBase<SuperClass>::CurvesControllers& CCurveEditorSplineControllerBase<SuperClass>::GetCurvesControllers() noexcept
{
    return m_CurvesControllers;
}

template<typename SuperClass>
void CCurveEditorSplineControllerBase<SuperClass>::CreateComponentsControllers()
{
    const auto createControllers = [this](const auto controllersCount, const auto addMethod)
    {
        for (auto i = 0u; i < controllersCount; ++i)
        {
            const auto addedController = (this->*addMethod)(i);
            EDITOR_ASSERT(addedController);
        }
    };

    createControllers(CalculateKnotsCount(), &CCurveEditorSplineControllerBase<SuperClass>::AddKnotController);
    createControllers(CalculateTangentsCount(), &CCurveEditorSplineControllerBase<SuperClass>::AddTangentController);
    createControllers(CalculateCurvesCount(), &CCurveEditorSplineControllerBase<SuperClass>::AddCurveController);
}

template<typename SuperClass>
void CCurveEditorSplineControllerBase<SuperClass>::DestroyComponentsControllers()
{
    const auto notifyDestroyControllers = [this](const auto& container, const auto method)
    {
        VisitObjectsContainer(container, [&](const auto& controlller)
        {
            NotifyListeners(method, controlller);
        });
    };

    notifyDestroyControllers(m_CurvesControllers, &ICurveEditorSplineControllerListener::OnCurveDestroyed);
    notifyDestroyControllers(m_TangentsControllers, &ICurveEditorSplineControllerListener::OnTangentCreated);
    notifyDestroyControllers(m_KnotsControllers, &ICurveEditorSplineControllerListener::OnKnotDestroyed);

    m_CurvesControllers.clear();
    m_TangentsControllers.clear();
    m_KnotsControllers.clear();
}

template<typename ControllerType, typename DataModelType, typename Method>
static auto CreateSplineControllerComponent(const DataModelType& dataModel, const Method setMethod, size_t controlPointIndex)
{
    auto result = std::shared_ptr<ControllerType>(ControllerType::Create());

    if (result)
    {
        auto isValid = true;

        isValid &= result->SetDataModel(dataModel);
        isValid &= (result.get()->*setMethod)(controlPointIndex);
        EDITOR_ASSERT(isValid);

        if (!isValid)
            result.reset();
    }

    return result;
}

template<typename SuperClass>
ICurveEditorKnotControllerPrivateSharedPtr CCurveEditorSplineControllerBase<SuperClass>::AddKnotController(size_t knotIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorKnotControllerPrivate>(GetDataModel(), &ICurveEditorKnotControllerPrivate::SetKnotIndex, knotIndex);
    if (result)
    {
        m_KnotsControllers.emplace_back(result);
        NotifyListeners(&ICurveEditorSplineControllerListener::OnKnotCreated, result);
    }

    return result;
}

template<typename SuperClass>
bool CCurveEditorSplineControllerBase<SuperClass>::RemoveKnotController(const ICurveEditorKnotControllerPrivateSharedPtr& knotController)
{
    const auto result = RemoveFromContainer(m_KnotsControllers, knotController);
    if(result)
        NotifyListeners(&ICurveEditorSplineControllerListener::OnKnotDestroyed, knotController);

    return result;
}

template<typename SuperClass>
ICurveEditorTangentControllerPrivateSharedPtr CCurveEditorSplineControllerBase<SuperClass>::AddTangentController(size_t tangentIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorTangentControllerPrivate>(GetDataModel(), &ICurveEditorTangentControllerPrivate::SetTangentIndex, tangentIndex);
    if (result)
    {
        m_TangentsControllers.emplace_back(result);
        NotifyListeners(&ICurveEditorSplineControllerListener::OnTangentCreated, result);
    }

    return result;
}

template<typename SuperClass>
bool CCurveEditorSplineControllerBase<SuperClass>::RemoveTangentController(const ICurveEditorTangentControllerPrivateSharedPtr& tangentController)
{
    const auto result = RemoveFromContainer(m_TangentsControllers, tangentController);
    if (result)
        NotifyListeners(&ICurveEditorSplineControllerListener::OnTangentDestroyed, tangentController);

    return result;
}

template<typename SuperClass>
ICurveEditorCurveControllerPrivateSharedPtr CCurveEditorSplineControllerBase<SuperClass>::AddCurveController(size_t curveIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorCurveControllerPrivate>(GetDataModel(), &ICurveEditorCurveControllerPrivate::SetCurveIndex, curveIndex);
    if (result)
    {
        m_CurvesControllers.emplace_back(result);
        NotifyListeners(&ICurveEditorSplineControllerListener::OnCurveCreated, result);
    }

    return result;
}

template<typename SuperClass>
bool CCurveEditorSplineControllerBase<SuperClass>::RemoveCurveController(const ICurveEditorCurveControllerPrivateSharedPtr& curveController)
{
    const auto result = RemoveFromContainer(m_CurvesControllers, curveController);
    if (result)
        NotifyListeners(&ICurveEditorSplineControllerListener::OnCurveDestroyed, curveController);

    return result;
}