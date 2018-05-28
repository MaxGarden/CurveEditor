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

    void OnKnotInserted(size_t controlPointIndex);
    void OnKnotRemoved(size_t controlPointIndex);

    void OnControlPointsPositionsChanged();

protected:
    virtual void OnDataModelChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

private:
    virtual size_t GetKnotsCount() const noexcept;
    virtual size_t GetTangentsCount() const noexcept;
    virtual size_t GetCurvesCount() const noexcept;

    template<typename Container, typename GetMethod>
    auto SortComponents(Container& container, GetMethod getPositionMethod) const;

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
    ICurveEditorTangentControllerPrivateSharedPtr AddTangentController(size_t tangentIndex);
    ICurveEditorCurveControllerPrivateSharedPtr AddCurveController(size_t curveIndex);

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
    bool m_EdititngMode = false;
};

class CCurveEditorFunctionSplineControllerListener final : public CCurveEditorSplineDataModelListenerBase
{
public:
    CCurveEditorFunctionSplineControllerListener(CCurveEditorFunctionSplineController& functionSplineController);
    virtual ~CCurveEditorFunctionSplineControllerListener() override final = default;

    virtual void OnKnotInserted(size_t controlPointIndex) override final;
    virtual void OnKnotRemoved(size_t controlPointIndex) override final;

    virtual void OnControlPointsPositionsChanged(const SplineControlPointsPositions& positions) override final;

private:
    CCurveEditorFunctionSplineController& m_FunctionSplineController;
};

CCurveEditorFunctionSplineControllerListener::CCurveEditorFunctionSplineControllerListener(CCurveEditorFunctionSplineController& functionSplineController) :
    m_FunctionSplineController(functionSplineController)
{
}

void CCurveEditorFunctionSplineControllerListener::OnKnotInserted(size_t controlPointIndex)
{
    m_FunctionSplineController.OnKnotInserted(controlPointIndex);
}

void CCurveEditorFunctionSplineControllerListener::OnKnotRemoved(size_t controlPointIndex)
{
    m_FunctionSplineController.OnKnotRemoved(controlPointIndex);
}

void CCurveEditorFunctionSplineControllerListener::OnControlPointsPositionsChanged(const SplineControlPointsPositions&)
{
    m_FunctionSplineController.OnControlPointsPositionsChanged();
}

bool CCurveEditorFunctionSplineController::BeginEditing()
{
    if (m_EdititngMode)
        return false;

    m_EdititngMode = true;
    return true;
}

bool CCurveEditorFunctionSplineController::EndEditing()
{
    if (!m_EdititngMode)
        return false;

    m_EdititngMode = false;

    if (m_QueuedControlPointsChange)
        OnControlPointsPositionsChanged();

    return true;
}

bool CCurveEditorFunctionSplineController::SaveState()
{
    if (!(m_SavedControlPointsPositions.empty() && m_SavedKnotsControllers.empty() && m_SavedTangentsControllers.empty()))
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

void CCurveEditorFunctionSplineController::OnKnotInserted(size_t)
{
    //TODO
    EDITOR_ASSERT(false);
}

void CCurveEditorFunctionSplineController::OnKnotRemoved(size_t)
{
    //TODO
    EDITOR_ASSERT(false);
}

void CCurveEditorFunctionSplineController::OnControlPointsPositionsChanged()
{
    if (m_EdititngMode)
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
auto CCurveEditorFunctionSplineController::SortComponents(Container& container, GetMethod getPositionMethod) const
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
    if (!SortComponents(m_TangentsControllers, &ICurveEditorTangentController::GetAnchorPosition))
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

        EDITOR_ASSERT(newTangentIndex % 2 == *previousTangentIndex % 2);

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
    if (!SortComponents(m_KnotsControllers, &ICurveEditorKnotController::GetPosition))
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
    const auto createControllers = [this](const auto controllersCount, const auto addMethod, const auto listenerMethod)
    {
        for (auto i = 0u; i < controllersCount; ++i)
        {
            const auto addedController = (this->*addMethod)(i);
            EDITOR_ASSERT(addedController);
            if (!addedController)
                continue;

            NotifyListeners(listenerMethod, addedController);
        }
    };

    createControllers(GetKnotsCount(), &CCurveEditorFunctionSplineController::AddKnotController, &ICurveEditorSplineControllerListener::OnKnotCreated);
    createControllers(GetTangentsCount(), &CCurveEditorFunctionSplineController::AddTangentController, &ICurveEditorSplineControllerListener::OnTangentCreated);
    createControllers(GetCurvesCount(), &CCurveEditorFunctionSplineController::AddCurveController, &ICurveEditorSplineControllerListener::OnCurveCreated);
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
        m_KnotsControllers.emplace_back(result);

    return result;
}

ICurveEditorTangentControllerPrivateSharedPtr CCurveEditorFunctionSplineController::AddTangentController(size_t tangentIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorTangentControllerPrivate>(GetDataModel(), &ICurveEditorTangentControllerPrivate::SetTangentIndex, tangentIndex);
    if (result)
        m_TangentsControllers.emplace_back(result);

    return result;
}

ICurveEditorCurveControllerPrivateSharedPtr CCurveEditorFunctionSplineController::AddCurveController(size_t curveIndex)
{
    auto result = CreateSplineControllerComponent<ICurveEditorCurveControllerPrivate>(GetDataModel(), &ICurveEditorCurveControllerPrivate::SetCurveIndex, curveIndex);
    if (result)
        m_CurvesControllers.emplace_back(result);

    return result;
}

ICurveEditorFunctionSplineControllerUniquePtr ICurveEditorFunctionSplineController::Create()
{
    return std::make_unique<CCurveEditorFunctionSplineController>();
}