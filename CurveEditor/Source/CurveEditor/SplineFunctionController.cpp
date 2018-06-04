#include "pch.h"
#include "SplineFunctionController.h"
#include "SplineControllerBase.h"

class CCurveEditorFunctionSplineController final : public CCurveEditorSplineControllerBase<ICurveEditorFunctionSplineController>
{
public:
    CCurveEditorFunctionSplineController() = default;
    virtual ~CCurveEditorFunctionSplineController() override final = default;

    virtual bool BeginEditing() override final;
    virtual bool EndEditing() override final;

    void OnControlPointsPositionsChanged();

protected:
    virtual IEditorListenerUniquePtr CreateListener() override final;

private:
    void ComponentsReplacement();
    void TangentsReplacement();
    void TangentExtremeCase();
    void KnotsReplacement();

    void TangentsConstraint();

private:
    bool m_BlockControlPointsPoisitonChangedEvent = false;

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

void CCurveEditorFunctionSplineController::OnControlPointsPositionsChanged()
{
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

IEditorListenerUniquePtr CCurveEditorFunctionSplineController::CreateListener()
{
    return std::make_unique<CCurveEditorFunctionSplineControllerListener>(*this);
}

void CCurveEditorFunctionSplineController::ComponentsReplacement()
{
    TangentsReplacement();
    KnotsReplacement();
}

void CCurveEditorFunctionSplineController::TangentsReplacement()
{
    auto& tangentsControllers = GetTangentsControllers();

    if (!SortComponentsByPosition(tangentsControllers, &ICurveEditorTangentController::GetAnchorPosition))
        return;

    using TangentPair = std::pair<ICurveEditorTangentController*, ax::pointf>;
    std::vector<TangentPair> tangentsToUpdate;

    TangentExtremeCase();

    VisitPointersContainer(tangentsControllers, [&tangentsToUpdate, tangentIndex = 0u](auto& tangentController) mutable
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
    auto& tangentsControllers = GetTangentsControllers();

    if (tangentsControllers.size() <= 3)
        return;

    auto firstIndex = 0u;
    for (; firstIndex < tangentsControllers.size(); firstIndex += 2)
    {
        const auto iterator = tangentsControllers.begin() + firstIndex;
        const auto currentTangentIndex = (*iterator)->GetIndex();
        EDITOR_ASSERT(currentTangentIndex);

        if (currentTangentIndex && *currentTangentIndex == 0)
            break;

        std::iter_swap(iterator, iterator + 1);
    }

    const auto lastTangentIndex = tangentsControllers.size() - 1;
    const auto tangentControllerToCheckCount = tangentsControllers.size() - firstIndex;

    for (auto secondIndex = 0u; secondIndex < tangentControllerToCheckCount; secondIndex += 2)
    {
        const auto iterator = tangentsControllers.rbegin() + secondIndex;
        const auto currentTangentIndex = (*iterator)->GetIndex();
        EDITOR_ASSERT(currentTangentIndex);

        if (currentTangentIndex && (*currentTangentIndex == lastTangentIndex))
            break;

        std::iter_swap(iterator, iterator + 1);
    }
}

void CCurveEditorFunctionSplineController::KnotsReplacement()
{
    auto& knotsControllers = GetKnotsControllers();

    if (!SortComponentsByPosition(knotsControllers, &ICurveEditorKnotController::GetPosition))
        return;

    using KnotPair = std::pair<ICurveEditorKnotController*, ax::pointf>;
    std::vector<KnotPair> knotsToUpdate;

    VisitPointersContainer(knotsControllers, [&knotsToUpdate, knotIndex = 0u](auto& knotController) mutable
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

ICurveEditorFunctionSplineControllerUniquePtr ICurveEditorFunctionSplineController::Create()
{
    return std::make_unique<CCurveEditorFunctionSplineController>();
}