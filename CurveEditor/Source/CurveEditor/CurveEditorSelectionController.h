#pragma  once
#if !defined(__CURVE_EDITOR_SELECTION_CONTROLLER_MODEL_H__)

#include "CurveEditorSelectionDataModel.h"

using CurveEditorControllerSelection = std::set<ICurveEditorSplineComponentControllerSharedPtr>;

class ICurveEditorSelectionControllerListener : public IEditorListener
{
public:
    virtual ~ICurveEditorSelectionControllerListener() override = default;

    virtual void OnAddedToSelection(const CurveEditorControllerSelection& selection) = 0;
    virtual void OnRemovedFromSelection(const CurveEditorControllerSelection& selection, bool clear) = 0;
};

class ICurveEditorSelectionController : public IEditorController
{
public:
    virtual ~ICurveEditorSelectionController() override = default;

    virtual bool SetSelectionMode(ECurveEditorSplineComponentType selectionMode) = 0;
    virtual std::optional<ECurveEditorSplineComponentType> GetSelectionMode() const noexcept = 0;

    virtual bool AddToSelection(const CurveEditorDataModelSelection& selection) = 0;
    virtual bool RemoveFromSelection(const CurveEditorDataModelSelection& selection) = 0;
    virtual bool ClearSelection() = 0;

    virtual bool CheckIfSelected(const CurveEditorDataModelSingleSelection& singleSelection) const noexcept = 0;

    virtual void VisitSelection(const ConstVisitorType<ICurveEditorSplineComponentControllerSharedPtr>& visitor) const = 0;
};

class ICurveEditorSelectionControllerPrivate : public ICurveEditorSelectionController
{
public:
    virtual ~ICurveEditorSelectionControllerPrivate() override = default;

    virtual bool RegisterSpline(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
    virtual bool UnregisterSpline(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;

    static ICurveEditorSelectionControllerPrivateUniquePtr Create();
};

#endif //__CURVE_EDITOR_SELECTION_CONTROLLER_MODEL_H__