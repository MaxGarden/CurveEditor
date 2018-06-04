#include "pch.h"
#include "SplineFunctionController.h"
#include "CurveEditorController.h"
#include "CurveEditorSplineListenerBase.h"
#include "KnotController.h"
#include "TangentController.h"
#include "CurveController.h"

class CCurveEditorFunctionSplineController final : public CEditorControllerBase<ICurveEditorFunctionSplineController, ICurveEditorSplineDataModel, ICurveEditorSplineControllerListener>
{
public:
    CCurveEditorFunctionSplineController() = default;
    virtual ~CCurveEditorFunctionSplineController() override final = default;

    virtual bool BeginEditing() override final;
    virtual bool EndEditing() override final;

    virtual bool SaveState() override final;
    virtual bool RestoreState() override final;
    virtual void ResetSavedState() noexcept override final;

    virtual const SplineID& GetID() const noexcept override final;
    virtual const SplineColor& GetColor() const noexcept override final;

    virtual void VisitKnotsControllers(const ConstVisitorType<ICurveEditorKnotControllerSharedPtr>& visitor) const override final;
    virtual void VisitTangentsControllers(const ConstVisitorType<ICurveEditorTangentControllerSharedPtr>& visitor) const override final;
    virtual void VisitCurvesControllers(const ConstVisitorType<ICurveEditorCurveControllerSharedPtr>& visitor) const override final;

    virtual ICurveEditorSplineComponentControllerSharedPtr GetSplineComponentController(ECurveEditorSplineComponentType type, size_t index) const noexcept override final;

    void OnControlPointsAdded(const SplineControlPointsPositions& positions);
    void OnControlPointsRemoved(const SplineControlPointsIndexes& indexes);

    void OnControlPointsPositionsChanged();

protected:
    virtual void OnDataModelChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

private:
    virtual size_t GetKnotsCount() const noexcept;
    virtual size_t GetTangentsCount() const noexcept;
    virtual size_t GetCurvesCount() const noexcept;

    template<typename Container, typename GetMethod>
    bool SortComponentsByPosition(Container& container, GetMethod getPositionMethod) const;

    template<typename Container>
    bool SortComponentsByIndex(Container& container) const;

    template<typename Container>
    void UpdateComponents(Container& container) const;

    void ComponentsReplacement();
    void TangentsReplacement();
    void TangentExtremeCase();
    void KnotsReplacement();

    void TangentsConstraint();

    const std::vector<ax::pointf>& GetControlPoints() const noexcept;

    void CreateControllers();
    void DestroyControllers();

    ICurveEditorKnotControllerPrivateSharedPtr AddKnotController(size_t knotIndex);
    bool RemoveKnotController(const ICurveEditorKnotControllerPrivateSharedPtr& knotController);

    ICurveEditorTangentControllerPrivateSharedPtr AddTangentController(size_t tangentIndex);
    bool RemoveTangentController(const ICurveEditorTangentControllerPrivateSharedPtr& tangentController);

    ICurveEditorCurveControllerPrivateSharedPtr AddCurveController(size_t curveIndex);
    bool RemoveCurveController(const ICurveEditorCurveControllerPrivateSharedPtr& curveController);

private:
    using KnotsControllers = std::vector<ICurveEditorKnotControllerPrivateSharedPtr>;
    using TangentsControllers = std::vector<ICurveEditorTangentControllerPrivateSharedPtr>;
    using CurvesControllers = std::vector<ICurveEditorCurveControllerPrivateSharedPtr>;

    KnotsControllers m_KnotsControllers;
    TangentsControllers m_TangentsControllers;
    CurvesControllers m_CurvesControllers;

    bool m_BlockControlPointsPoisitonChangedEvent = false;

    SplineControlPointsPositions m_SavedControlPointsPositions;
    KnotsControllers m_SavedKnotsControllers;
    TangentsControllers m_SavedTangentsControllers;

    bool m_QueuedControlPointsChange = false;
    bool m_EditingMode = false;
};

class CCurveEditorFunctionSplineControllerListener final : public CCurveEditorSplineDataModelListenerBase
{
public:
    CCurveEditorFunctionSplineControllerListener(CCurveEditorFunctionSplineController& functionSplineController);
    virtual ~CCurveEditorFunctionSplineControllerListener() override final = default;

    virtual void OnControlPointsAdded(const SplineControlPointsPositions& positions) override final;
    virtual void OnControlPointsRemoved(const SplineControlPointsIndexes& indexes) override final;

    virtual void OnControlPointsModified(const SplineControlPointsPositions& positions) override final;

private:
    CCurveEditorFunctionSplineController& m_FunctionSplineController;
};

CCurveEditorFunctionSplineControllerListener::CCurveEditorFunctionSplineControllerListener(CCurveEditorFunctionSplineController& functionSplineController) :
    m_FunctionSplineController(functionSplineController)
{
}

void CCurveEditorFunctionSplineControllerListener::OnControlPointsAdded(const SplineControlPointsPositions& positions)
{
    m_FunctionSplineController.OnControlPointsAdded(positions);
}

void CCurveEditorFunctionSplineControllerListener::OnControlPointsRemoved(const SplineControlPointsIndexes& indexes)
{
    m_FunctionSplineController.OnControlPointsRemoved(indexes);
}

void CCurveEditorFunctionSplineControllerListener::OnControlPointsModified(const SplineControlPointsPositions&)
{
    m_FunctionSplineController.OnControlPointsPositionsChanged();
}

bool CCurveEditorFunctionSplineController::BeginEditing()
{
    if (m_EditingMode)
        return false;

    m_EditingMode = true;
    return true;
}

bool CCurveEditorFunctionSplineController::EndEditing()
{
    if (!m_EditingMode)
        return false;

    m_EditingMode = false;

    if (m_QueuedControlPointsChange)
        OnControlPointsPositionsChanged();

    return true;
}

bool CCurveEditorFunctionSplineController::SaveState()
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

bool CCurveEditorFunctionSplineController::RestoreState()
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

void CCurveEditorFunctionSplineController::ResetSavedState() noexcept
{
    m_SavedControlPointsPositions.clear();
    m_SavedKnotsControllers.clear();
    m_SavedTangentsControllers.clear();
}

const SplineID& CCurveEditorFunctionSplineController::GetID() const noexcept
{
    static const SplineID null = ICurveEditorSplineDataModel::InvalidSplineID();

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetID();

    return null;
}

const SplineColor& CCurveEditorFunctionSplineController::GetColor() const noexcept
{
    static const SplineColor null = {};

    if (const auto dataModel = GetDataModel())
        return dataModel->GetColor();

    return null;
}

void CCurveEditorFunctionSplineController::VisitKnotsControllers(const ConstVisitorType<ICurveEditorKnotControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_KnotsControllers, visitor);
}

void CCurveEditorFunctionSplineController::VisitTangentsControllers(const ConstVisitorType<ICurveEditorTangentControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_TangentsControllers, visitor);
}

void CCurveEditorFunctionSplineController::VisitCurvesControllers(const ConstVisitorType<ICurveEditorCurveControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_CurvesControllers, visitor);
}

ICurveEditorSplineComponentControllerSharedPtr CCurveEditorFunctionSplineController::GetSplineComponentController(ECurveEditorSplineComponentType type, size_t index) const noexcept
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

void CCurveEditorFunctionSplineController::OnControlPointsAdded(const SplineControlPointsPositions& positions)
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

    insertSplineComponentsControllers(m_KnotsControllers, GetKnotsCount(), &ICurveEditorKnotControllerPrivate::SetKnotIndex, &CCurveEditorFunctionSplineController::AddKnotController);
    insertSplineComponentsControllers(m_TangentsControllers, GetTangentsCount(), &ICurveEditorTangentControllerPrivate::SetTangentIndex, &CCurveEditorFunctionSplineController::AddTangentController);
    insertSplineComponentsControllers(m_CurvesControllers, GetCurvesCount(), &ICurveEditorCurveControllerPrivate::SetCurveIndex, &CCurveEditorFunctionSplineController::AddCurveController);
}

void CCurveEditorFunctionSplineController::OnControlPointsRemoved(const SplineControlPointsIndexes& indexes)
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

    removeSplineComponentsController(m_KnotsControllers, GetKnotsCount(), &ICurveEditorKnotControllerPrivate::SetKnotIndex, &CCurveEditorFunctionSplineController::RemoveKnotController);
    removeSplineComponentsController(m_TangentsControllers, GetTangentsCount(), &ICurveEditorTangentControllerPrivate::SetTangentIndex, &CCurveEditorFunctionSplineController::RemoveTangentController);
    removeSplineComponentsController(m_CurvesControllers, GetCurvesCount(), &ICurveEditorCurveControllerPrivate::SetCurveIndex, &CCurveEditorFunctionSplineController::RemoveCurveController);
}

void CCurveEditorFunctionSplineController::OnControlPointsPositionsChanged()
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if(dataModel && dataModel->GetType() != ECurveEditorSplineType::Function)
        return;

    if (m_EditingMode)
    {
        m_QueuedControlPointsChange = true;
        return;
    }

    if (m_BlockControlPointsPoisitonChangedEvent)
        return;

    m_BlockControlPointsPoisitonChangedEvent = true;

    CScopedGuard guard{ [this]()
    {
        m_BlockControlPointsPoisitonChangedEvent = false;
    } };

    ComponentsReplacement();
    TangentsConstraint();

    m_QueuedControlPointsChange = false;
}

void CCurveEditorFunctionSplineController::OnDataModelChanged()
{
    DestroyControllers();
    CreateControllers();
}

IEditorListenerUniquePtr CCurveEditorFunctionSplineController::CreateListener()
{
    return std::make_unique<CCurveEditorFunctionSplineControllerListener>(*this);
}

size_t CCurveEditorFunctionSplineController::GetKnotsCount() const noexcept
{
    const auto curvesCount = GetCurvesCount();
    if (curvesCount == 0)
        return 0;

    return curvesCount + 1;
}

size_t CCurveEditorFunctionSplineController::GetTangentsCount() const noexcept
{
    return GetCurvesCount() * 2;
}

size_t CCurveEditorFunctionSplineController::GetCurvesCount() const noexcept
{
    const auto controlPointsPerCurve = ControlPointsPerCurve();

    const auto controlPointsCount = GetControlPoints().size();
    if (controlPointsCount < controlPointsPerCurve)
        return 0;

    return 1 + (controlPointsCount - controlPointsPerCurve) / (controlPointsPerCurve - 1);
}

template<typename Container, typename GetMethod>
bool CCurveEditorFunctionSplineController::SortComponentsByPosition(Container& container, GetMethod getPositionMethod) const
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

template<typename Container>
bool CCurveEditorFunctionSplineController::SortComponentsByIndex(Container& container) const
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


template<typename Container>
void CCurveEditorFunctionSplineController::UpdateComponents(Container& container) const
{
    for (const auto& componentPair : container)
    {
        const auto result = componentPair.first->SetPosition(componentPair.second);
        EDITOR_ASSERT(result);
    }
}

void CCurveEditorFunctionSplineController::ComponentsReplacement()
{
    TangentsReplacement();
    KnotsReplacement();
}

void CCurveEditorFunctionSplineController::TangentsReplacement()
{
    if (!SortComponentsByPosition(m_TangentsControllers, &ICurveEditorTangentController::GetAnchorPosition))
        return;

    using TangentPair = std::pair<ICurveEditorTangentController*, ax::pointf>;
    std::vector<TangentPair> tangentsToUpdate;

    TangentExtremeCase();

    VisitPointersContainer(m_TangentsControllers, [&tangentsToUpdate, tangentIndex = 0u](auto& tangentController) mutable
    {
        const auto tangentPosition = tangentController.GetPosition();
        EDITOR_ASSERT(tangentPosition);
        if (!tangentPosition)
            return;

        const auto previousAnchorPosition = tangentController.GetAnchorPosition();
        EDITOR_ASSERT(previousAnchorPosition);
        if (!previousAnchorPosition)
            return;

        const auto newTangentIndex = tangentIndex++;
        const auto previousTangentIndex = tangentController.GetIndex();

        if (previousTangentIndex && *previousTangentIndex == newTangentIndex)
            return;

        const auto result = tangentController.SetTangentIndex(newTangentIndex);
        EDITOR_ASSERT(result);
        if (!result)
            return;

        const auto newAnchorPosition = tangentController.GetAnchorPosition();
        EDITOR_ASSERT(newAnchorPosition);
        if (!newAnchorPosition)
            return;

        tangentsToUpdate.emplace_back(&tangentController, *newAnchorPosition + (*tangentPosition - *previousAnchorPosition));
    });

    UpdateComponents(tangentsToUpdate);
}

void CCurveEditorFunctionSplineController::TangentExtremeCase()
{
    if (m_TangentsControllers.size() <= 3)
        return;

    auto firstIndex = 0u;
    for (; firstIndex < m_TangentsControllers.size(); firstIndex += 2)
    {
        const auto iterator = m_TangentsControllers.begin() + firstIndex;
        const auto currentTangentIndex = (*iterator)->GetIndex();
        EDITOR_ASSERT(currentTangentIndex);

        if (currentTangentIndex && *currentTangentIndex == 0)
            break;

        std::iter_swap(iterator, iterator + 1);
    }

    const auto lastTangentIndex = m_TangentsControllers.size() - 1;
    const auto tangentControllerToCheckCount = m_TangentsControllers.size() - firstIndex;

    for (auto secondIndex = 0u; secondIndex < tangentControllerToCheckCount; secondIndex += 2)
    {
        const auto iterator = m_TangentsControllers.rbegin() + secondIndex;
        const auto currentTangentIndex = (*iterator)->GetIndex();
        EDITOR_ASSERT(currentTangentIndex);

        if (currentTangentIndex && (*currentTangentIndex == lastTangentIndex))
            break;

        std::iter_swap(iterator, iterator + 1);
    }
}

void CCurveEditorFunctionSplineController::KnotsReplacement()
{
    if (!SortComponentsByPosition(m_KnotsControllers, &ICurveEditorKnotController::GetPosition))
        return;

    using KnotPair = std::pair<ICurveEditorKnotController*, ax::pointf>;
    std::vector<KnotPair> knotsToUpdate;

    VisitPointersContainer(m_KnotsControllers, [&knotsToUpdate, knotIndex = 0u](auto& knotController) mutable
    {
        const auto knotPosition = knotController.GetPosition();
        EDITOR_ASSERT(knotPosition);
        if (!knotPosition)
            return;

        const auto newKnotIndex = knotIndex++;
        if (const auto currentKnotIndex = knotController.GetIndex(); currentKnotIndex && *currentKnotIndex == newKnotIndex)
            return;

        const auto result = knotController.SetKnotIndex(newKnotIndex);
        EDITOR_ASSERT(result);
        if (!result)
            return;

        knotsToUpdate.emplace_back(&knotController, *knotPosition);
    });

    UpdateComponents(knotsToUpdate);
}

void CCurveEditorFunctionSplineController::TangentsConstraint()
{
    ICurveEditorTangentController* previousTangentController = nullptr;

    VisitTangentsControllers([&](const auto& tangentController)
    {
        EDITOR_ASSERT(tangentController);
        if (!tangentController)
            return;

        const auto tangentIndex = tangentController->GetIndex();
        EDITOR_ASSERT(tangentIndex);
        if (!tangentIndex)
            return;

        const auto tangentPosition = tangentController->GetPosition();
        EDITOR_ASSERT(tangentPosition);
        if (!tangentPosition)
            return;

        const auto anchorPosition = tangentController->GetAnchorPosition();
        EDITOR_ASSERT(anchorPosition);
        if (!anchorPosition)
            return;

        const auto isTangentLeftLocated = (*tangentIndex % 2) == 1;

        if (isTangentLeftLocated)
        {
            if (tangentPosition->x > anchorPosition->x)
                tangentController->SetPosition({ anchorPosition->x, tangentPosition->y });

            if (EDITOR_ASSERT(previousTangentController);  previousTangentController)
            {
                const auto previousTangentAnchorPosition = previousTangentController->GetAnchorPosition();
                EDITOR_ASSERT(previousTangentAnchorPosition);
                if (!previousTangentAnchorPosition)
                    return;

                if (tangentPosition->x < previousTangentAnchorPosition->x)
                    tangentController->SetPosition({ previousTangentAnchorPosition->x, tangentPosition->y });

                const auto previousTangentPosition = previousTangentController->GetPosition();
                EDITOR_ASSERT(previousTangentPosition);
                if (!previousTangentPosition)
                    return;

                if (previousTangentPosition->x > anchorPosition->x)
                    previousTangentController->SetPosition({ anchorPosition->x, previousTangentPosition->y });
            }
        }
        else
        {
            if (tangentPosition->x < anchorPosition->x)
                tangentController->SetPosition({ anchorPosition->x, tangentPosition->y });
        }

        previousTangentController = tangentController.get();
    });
}

const std::vector<ax::pointf>& CCurveEditorFunctionSplineController::GetControlPoints() const noexcept
{
    static const std::vector<ax::pointf> null;

    if (const auto& dataModel = GetDataModel())
        return dataModel->GetControlPoints();

    return null;
}

void CCurveEditorFunctionSplineController::CreateControllers()
{
    const auto createControllers = [this](const auto controllersCount, const auto addMethod)
    {
        for (auto i = 0u; i < controllersCount; ++i)
        {
            const auto addedController = (this->*addMethod)(i);
            EDITOR_ASSERT(addedController);
        }
    };

    createControllers(GetKnotsCount(), &CCurveEditorFunctionSplineController::AddKnotController);
    createControllers(GetTangentsCount(), &CCurveEditorFunctionSplineController::AddTangentController);
    createControllers(GetCurvesCount(), &CCurveEditorFunctionSplineController::AddCurveController);
}

void CCurveEditorFunctionSplineController::DestroyControllers()
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

ICurveEditorKnotControllerPrivateSharedPtr CCurveEditorFunctionSplineController::AddKnotController(size_t knotIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorKnotControllerPrivate>(GetDataModel(), &ICurveEditorKnotControllerPrivate::SetKnotIndex, knotIndex);
    if (result)
    {
        m_KnotsControllers.emplace_back(result);
        NotifyListeners(&ICurveEditorSplineControllerListener::OnKnotCreated, result);
    }

    return result;
}

bool CCurveEditorFunctionSplineController::RemoveKnotController(const ICurveEditorKnotControllerPrivateSharedPtr& knotController)
{
    const auto result = RemoveFromContainer(m_KnotsControllers, knotController);
    if(result)
        NotifyListeners(&ICurveEditorSplineControllerListener::OnKnotDestroyed, knotController);

    return result;
}

ICurveEditorTangentControllerPrivateSharedPtr CCurveEditorFunctionSplineController::AddTangentController(size_t tangentIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorTangentControllerPrivate>(GetDataModel(), &ICurveEditorTangentControllerPrivate::SetTangentIndex, tangentIndex);
    if (result)
    {
        m_TangentsControllers.emplace_back(result);
        NotifyListeners(&ICurveEditorSplineControllerListener::OnTangentCreated, result);
    }

    return result;
}

bool CCurveEditorFunctionSplineController::RemoveTangentController(const ICurveEditorTangentControllerPrivateSharedPtr& tangentController)
{
    const auto result = RemoveFromContainer(m_TangentsControllers, tangentController);
    if (result)
        NotifyListeners(&ICurveEditorSplineControllerListener::OnTangentDestroyed, tangentController);

    return result;
}

ICurveEditorCurveControllerPrivateSharedPtr CCurveEditorFunctionSplineController::AddCurveController(size_t curveIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorCurveControllerPrivate>(GetDataModel(), &ICurveEditorCurveControllerPrivate::SetCurveIndex, curveIndex);
    if (result)
    {
        m_CurvesControllers.emplace_back(result);
        NotifyListeners(&ICurveEditorSplineControllerListener::OnCurveCreated, result);
    }

    return result;
}

bool CCurveEditorFunctionSplineController::RemoveCurveController(const ICurveEditorCurveControllerPrivateSharedPtr& curveController)
{
    const auto result = RemoveFromContainer(m_CurvesControllers, curveController);
    if (result)
        NotifyListeners(&ICurveEditorSplineControllerListener::OnCurveDestroyed, curveController);

    return result;
}

ICurveEditorFunctionSplineControllerUniquePtr ICurveEditorFunctionSplineController::Create()
{
    return std::make_unique<CCurveEditorFunctionSplineController>();
}