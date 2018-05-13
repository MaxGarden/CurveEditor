#include "pch.h"
#include "CurveEditorSelectionDataModel.h"
#include "EditorListenableBase.h"
#include "CurveEditorDataModel.h"
#include "SplineDataModel.h"
#include "CurveEditorListenerBase.h"

class CCurveEditorSelectionDataModel final : public CEditorListenableBase<ICurveEditorSelectionDataModel, ICurveEditorSelectionDataModelListener>
{
public:
    CCurveEditorSelectionDataModel() = default;
    virtual ~CCurveEditorSelectionDataModel() override final = default;

    virtual void SetSelectionMode(ECurveEditorSplineComponentType selectionMode) override final;
    virtual ECurveEditorSplineComponentType GetSelectionMode() const noexcept override final;

    virtual bool AddToSelection(const CurveEditorDataModelSelection& selection) override final;
    virtual bool RemoveFromSelection(const CurveEditorDataModelSelection& selection) override final;
    virtual void ClearSelection() override final;

    virtual const CurveEditorDataModelSelection& GetSelection() const noexcept override final;

private:
    ECurveEditorSplineComponentType m_SelectionMode = ECurveEditorSplineComponentType::Knot;

    ICurveEditorDataModelWeakPtr m_EditorDataModel;
    CurveEditorDataModelSelection m_Selection;
};

void CCurveEditorSelectionDataModel::SetSelectionMode(ECurveEditorSplineComponentType selectionMode)
{
    if (selectionMode == m_SelectionMode)
        return;

    ClearSelection();
    m_SelectionMode = selectionMode;
    NotifyListeners(&ICurveEditorSelectionDataModelListener::OnSelectionModeChanged, m_SelectionMode);
}

ECurveEditorSplineComponentType CCurveEditorSelectionDataModel::GetSelectionMode() const noexcept
{
    return m_SelectionMode;
}

bool CCurveEditorSelectionDataModel::AddToSelection(const CurveEditorDataModelSelection& selection)
{
    CurveEditorDataModelSelection addedSelection;
    std::set_difference(selection.cbegin(), selection.cend(), m_Selection.cbegin(), m_Selection.cend(), std::inserter(addedSelection, addedSelection.cend()));
    EDITOR_ASSERT(addedSelection.size() == selection.size());

    if (addedSelection.empty())
        return false;

    m_Selection.insert(addedSelection.cbegin(), addedSelection.cend());
    NotifyListeners(&ICurveEditorSelectionDataModelListener::OnAddedToSelection, addedSelection);

    return true;
}

bool CCurveEditorSelectionDataModel::RemoveFromSelection(const CurveEditorDataModelSelection& selection)
{
    CurveEditorDataModelSelection removedSelection;
    std::set_intersection(m_Selection.cbegin(), m_Selection.cend(), selection.cbegin(), selection.cend(), std::inserter(removedSelection, removedSelection.end()));
    EDITOR_ASSERT(removedSelection.size() == selection.size());

    if (removedSelection.empty())
        return false;

    CurveEditorDataModelSelection buffer;
    buffer.swap(m_Selection);

    std::set_difference(buffer.cbegin(), buffer.cend(), removedSelection.cbegin(), removedSelection.cend(), std::inserter(m_Selection, m_Selection.end()));
    NotifyListeners(&ICurveEditorSelectionDataModelListener::OnRemovedFromSelection, removedSelection, m_Selection.empty());

    return true;
}

void CCurveEditorSelectionDataModel::ClearSelection()
{
    CurveEditorDataModelSelection buffer;
    m_Selection.swap(buffer);

    NotifyListeners(&ICurveEditorSelectionDataModelListener::OnRemovedFromSelection, buffer, true);
}

const CurveEditorDataModelSelection& CCurveEditorSelectionDataModel::GetSelection() const noexcept
{
    return m_Selection;
}

ICurveEditorSelectionDataModelUniquePtr ICurveEditorSelectionDataModel::Create()
{
    return std::make_unique<CCurveEditorSelectionDataModel>();
}