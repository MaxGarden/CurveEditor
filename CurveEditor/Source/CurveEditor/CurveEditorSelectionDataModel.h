#pragma  once
#if !defined(__CURVE_EDITOR_SELECTION_DATA_MODEL_H__)

#include "SplineDataModel.h"
#include "SplineController.h"

using CurveEditorDataModelSingleSelection = std::pair<SplineID, size_t>;
using CurveEditorDataModelSelection = std::set<CurveEditorDataModelSingleSelection>;

class ICurveEditorSelectionDataModelListener : public IEditorListener
{
public:
    virtual ~ICurveEditorSelectionDataModelListener() override = default;

    virtual void OnSelectionModeChanged(ECurveEditorSplineComponentType selectionMode) = 0;

    virtual void OnAddedToSelection(const CurveEditorDataModelSelection& selection) = 0;
    virtual void OnRemovedFromSelection(const CurveEditorDataModelSelection& selection, bool clear) = 0;
};

class ICurveEditorSelectionDataModel : public IEditorDataModel
{
public:
    virtual ~ICurveEditorSelectionDataModel() override = default;

    virtual void SetSelectionMode(ECurveEditorSplineComponentType selectionMode) = 0;
    virtual ECurveEditorSplineComponentType GetSelectionMode() const noexcept = 0;

    virtual bool AddToSelection(const CurveEditorDataModelSelection& selection) = 0;
    virtual bool RemoveFromSelection(const CurveEditorDataModelSelection& selection) = 0;
    virtual void ClearSelection() = 0;

    virtual const CurveEditorDataModelSelection& GetSelection() const noexcept = 0;

    static ICurveEditorSelectionDataModelUniquePtr Create();
};

#endif //__CURVE_EDITOR_SELECTION_DATA_MODEL_H__