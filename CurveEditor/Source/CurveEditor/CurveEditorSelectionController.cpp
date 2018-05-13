#include "pch.h"
#include "CurveEditorSelectionController.h"
#include "EditorControllerBase.h"
#include "CurveEditorSelectionListenerBase.h"
#include "CurveEditorController.h"
#include "SplineController.h"

class CCurveEditorSelectionController final : public CEditorControllerBase<ICurveEditorSelectionControllerPrivate, ICurveEditorSelectionDataModel, ICurveEditorSelectionControllerListener>
{
public:
    CCurveEditorSelectionController() = default;
    virtual ~CCurveEditorSelectionController() override final;

    virtual bool SetSelectionMode(ECurveEditorSplineComponentType selectionMode) override final;
    virtual std::optional<ECurveEditorSplineComponentType> GetSelectionMode() const noexcept override final;

    virtual bool AddToSelection(const CurveEditorDataModelSelection& selection) override final;
    virtual bool RemoveFromSelection(const CurveEditorDataModelSelection& selection) override final;
    virtual bool ClearSelection() override final;

    virtual bool CheckIfSelected(const CurveEditorDataModelSingleSelection& singleSelection) const noexcept override final;

    virtual void VisitSelection(const ConstVisitorType<ICurveEditorSplineComponentControllerSharedPtr>& visitor) const override final;

    virtual bool RegisterSpline(const ICurveEditorSplineControllerSharedPtr& splineController) override final;
    virtual bool UnregisterSpline(const ICurveEditorSplineControllerSharedPtr& splineController) override final;

    void OnSelectionModeChanged(ECurveEditorSplineComponentType selectionMode);

    bool OnAddedToSelection(const CurveEditorDataModelSelection& selection);
    bool OnRemovedFromSelection(const CurveEditorDataModelSelection& selection, bool clear);

protected:
    virtual void OnDataModelChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

private:
    ICurveEditorControllerSharedPtr GetEditorController() const noexcept;

    ICurveEditorSplineComponentControllerSharedPtr TransformDataModelSingleSelection(const CurveEditorDataModelSingleSelection& singleSelection) const noexcept;
    CurveEditorControllerSelection TransformDataModelSelection(const CurveEditorDataModelSelection& selection) const noexcept;

private:
    ICurveEditorControllerWeakPtr m_EditorController;
    CurveEditorControllerSelection m_Selection;

    std::optional<ECurveEditorSplineComponentType> m_SelectionMode;
    std::optional<EditorListenerHandle> m_EditorListenerHandle;
    std::unordered_map<SplineID, ICurveEditorSplineControllerSharedPtr> m_SplinesControllers;
};

class CCurveEditorSelectionControllerListener final : public CCurveEditorSelectionDataModelListenerBase
{
public:
    CCurveEditorSelectionControllerListener(CCurveEditorSelectionController& selectionController);
    virtual ~CCurveEditorSelectionControllerListener() override final = default;

    virtual void OnSelectionModeChanged(ECurveEditorSplineComponentType selectionMode) override final;

    virtual void OnAddedToSelection(const CurveEditorDataModelSelection& selection) override final;
    virtual void OnRemovedFromSelection(const CurveEditorDataModelSelection& selection, bool clear) override final;

private:
    CCurveEditorSelectionController& m_SelectionController;
};

CCurveEditorSelectionControllerListener::CCurveEditorSelectionControllerListener(CCurveEditorSelectionController& selectionController) :
    m_SelectionController(selectionController)
{
}

void CCurveEditorSelectionControllerListener::OnSelectionModeChanged(ECurveEditorSplineComponentType selectionMode)
{
    m_SelectionController.OnSelectionModeChanged(selectionMode);
}

void CCurveEditorSelectionControllerListener::OnAddedToSelection(const CurveEditorDataModelSelection& selection)
{
    const auto result = m_SelectionController.OnAddedToSelection(selection);
    EDITOR_ASSERT(result);
}

void CCurveEditorSelectionControllerListener::OnRemovedFromSelection(const CurveEditorDataModelSelection& selection, bool clear)
{
    const auto result = m_SelectionController.OnRemovedFromSelection(selection, clear);
    EDITOR_ASSERT(result);
}

CCurveEditorSelectionController::~CCurveEditorSelectionController()
{
    if (!m_EditorListenerHandle)
        return;

    const auto editorController = GetEditorController();
    if (!editorController)
        return;

    const auto result = editorController->UnregisterListener(*m_EditorListenerHandle);
    EDITOR_ASSERT(result);
}

bool CCurveEditorSelectionController::SetSelectionMode(ECurveEditorSplineComponentType selectionMode)
{
    if (const auto& dataModel = GetDataModel())
    {
        dataModel->SetSelectionMode(selectionMode);
        return true;
    }

    EDITOR_ASSERT(false);
    return false;
}

std::optional<ECurveEditorSplineComponentType> CCurveEditorSelectionController::GetSelectionMode() const noexcept
{
    if(const auto& dataModel = GetDataModel())
        return dataModel->GetSelectionMode();

    EDITOR_ASSERT(false);
    return std::nullopt;
}

bool CCurveEditorSelectionController::AddToSelection(const CurveEditorDataModelSelection& selection)
{
    if(const auto& dataModel = GetDataModel())
        return dataModel->AddToSelection(selection);

    EDITOR_ASSERT(false);
    return false;
}

bool CCurveEditorSelectionController::RemoveFromSelection(const CurveEditorDataModelSelection& selection)
{
    if (const auto& dataModel = GetDataModel())
        return dataModel->RemoveFromSelection(selection);

    EDITOR_ASSERT(false);
    return false;
}

bool CCurveEditorSelectionController::ClearSelection()
{
    if (const auto& dataModel = GetDataModel())
    {
        dataModel->ClearSelection();
        return true;
    }

    EDITOR_ASSERT(false);
    return false;
}

bool CCurveEditorSelectionController::CheckIfSelected(const CurveEditorDataModelSingleSelection& singleSelection) const noexcept
{
    const auto splineComponentController = TransformDataModelSingleSelection(singleSelection);
    EDITOR_ASSERT(splineComponentController);
    if (!splineComponentController)
        return false;

    return m_Selection.find(splineComponentController) != m_Selection.cend();
}

void CCurveEditorSelectionController::VisitSelection(const ConstVisitorType<ICurveEditorSplineComponentControllerSharedPtr>& visitor) const
{
    VisitObjectsContainer(m_Selection, visitor);
}

bool CCurveEditorSelectionController::RegisterSpline(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    if (!splineController)
        return false;

    const auto result = m_SplinesControllers.try_emplace(splineController->GetID(), splineController).second;
    EDITOR_ASSERT(result);

    return result;
}

bool CCurveEditorSelectionController::UnregisterSpline(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    if (!splineController)
        return false;

    const auto erasedCount = m_SplinesControllers.erase(splineController->GetID());
    EDITOR_ASSERT(erasedCount == 1);

    return erasedCount == 1;
}

void CCurveEditorSelectionController::OnSelectionModeChanged(ECurveEditorSplineComponentType selectionMode)
{
    m_SelectionMode = selectionMode;
}

bool CCurveEditorSelectionController::OnAddedToSelection(const CurveEditorDataModelSelection& selection)
{
    const auto transformedSelection = TransformDataModelSelection(selection);

    CurveEditorControllerSelection addedSelection;
    std::set_difference(transformedSelection.cbegin(), transformedSelection.cend(), m_Selection.cbegin(), m_Selection.cend(), std::inserter(addedSelection, addedSelection.end()));
    EDITOR_ASSERT(addedSelection.size() == selection.size());

    if (addedSelection.empty())
        return false;

    m_Selection.insert(addedSelection.cbegin(), addedSelection.cend());
    NotifyListeners(&ICurveEditorSelectionControllerListener::OnAddedToSelection, addedSelection);

    return true;
}

bool CCurveEditorSelectionController::OnRemovedFromSelection(const CurveEditorDataModelSelection& selection, bool clear)
{
    if (clear)
    {
        CurveEditorControllerSelection buffer;
        m_Selection.swap(buffer);

        NotifyListeners(&ICurveEditorSelectionControllerListener::OnRemovedFromSelection, buffer, true);
        return true;
    }

    const auto transformedSelection = TransformDataModelSelection(selection);

    CurveEditorControllerSelection removedSelection;
    std::set_intersection(transformedSelection.cbegin(), transformedSelection.cend(), m_Selection.cbegin(), m_Selection.cend(), std::inserter(removedSelection, removedSelection.end()));
    EDITOR_ASSERT(removedSelection.size() == selection.size());

    if (removedSelection.empty())
        return false;

    CurveEditorControllerSelection buffer;
    m_Selection.swap(buffer);

    std::set_difference(buffer.cbegin(), buffer.cend(), removedSelection.cbegin(), removedSelection.cend(), std::inserter(m_Selection, m_Selection.end()));
    NotifyListeners(&ICurveEditorSelectionControllerListener::OnRemovedFromSelection, removedSelection, m_Selection.empty());

    return true;
}

void CCurveEditorSelectionController::OnDataModelChanged()
{
    m_SelectionMode.reset();
    const auto clearResult = OnRemovedFromSelection({}, true);
    EDITOR_ASSERT(clearResult);
    if (!clearResult)
        m_Selection.clear();

    const auto& dataModel = GetDataModel();
    if (!dataModel)
        return;

    m_SelectionMode = dataModel->GetSelectionMode();

    if (const auto& selection = dataModel->GetSelection(); !selection.empty())
    {
        const auto addResult = OnAddedToSelection(selection);
        EDITOR_ASSERT(addResult);
    }
}

IEditorListenerUniquePtr CCurveEditorSelectionController::CreateListener()
{
    return std::make_unique<CCurveEditorSelectionControllerListener>(*this);
}

ICurveEditorControllerSharedPtr CCurveEditorSelectionController::GetEditorController() const noexcept
{
    const auto result = m_EditorController.lock();
    EDITOR_ASSERT(result);
    return result;
}

ICurveEditorSplineComponentControllerSharedPtr CCurveEditorSelectionController::TransformDataModelSingleSelection(const CurveEditorDataModelSingleSelection& singleSelection)  const noexcept
{
    EDITOR_ASSERT(m_SelectionMode);
    if (!m_SelectionMode)
        return nullptr;

    const auto& splineID = singleSelection.first;

    const auto iterator = m_SplinesControllers.find(splineID);
    EDITOR_ASSERT(iterator != m_SplinesControllers.cend());
    if (iterator == m_SplinesControllers.cend())
        return nullptr;

    const auto& splineController = iterator->second;
    EDITOR_ASSERT(splineController);
    if (!splineController)
        return nullptr;

    const auto& index = singleSelection.second;
    const auto& splineComponentController = splineController->GetSplineComponentController(*m_SelectionMode, index);
    EDITOR_ASSERT(splineComponentController);

    return splineComponentController;
}

CurveEditorControllerSelection CCurveEditorSelectionController::TransformDataModelSelection(const CurveEditorDataModelSelection& selection) const noexcept
{
    const auto& dataModel = GetDataModel();
    EDITOR_ASSERT(dataModel);
    if (!dataModel)
        return {};

    CurveEditorControllerSelection result;

    const auto selectionMode = dataModel->GetSelectionMode();

    for (const auto& singleSelection : selection)
    {
        const auto splineComponentController = TransformDataModelSingleSelection(singleSelection);
        EDITOR_ASSERT(splineComponentController);
        if (!splineComponentController)
            return {};

        result.emplace(splineComponentController);
    }

    return result;
}

ICurveEditorSelectionControllerPrivateUniquePtr ICurveEditorSelectionControllerPrivate::Create()
{
    return std::make_unique<CCurveEditorSelectionController>();
}