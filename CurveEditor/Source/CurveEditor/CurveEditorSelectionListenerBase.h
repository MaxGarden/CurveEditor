#pragma  once
#if !defined(__CURVE_EDITOR_SELECTION_LISTENER_BASE_H__)

#include "CurveEditorSelectionController.h"

class CCurveEditorSelectionDataModelListenerBase : public ICurveEditorSelectionDataModelListener
{
public:
    CCurveEditorSelectionDataModelListenerBase() = default;
    virtual ~CCurveEditorSelectionDataModelListenerBase() override = default;

    virtual void OnSelectionModeChanged(ECurveEditorSplineComponentType selectionMode) override;

    virtual void OnAddedToSelection(const CurveEditorDataModelSelection& selection) override;
    virtual void OnRemovedFromSelection(const CurveEditorDataModelSelection& selection, bool clear) override;
};

class CCurveEditorSelectionControllerListenerBase: public ICurveEditorSelectionControllerListener
{
public:
    CCurveEditorSelectionControllerListenerBase() = default;
    virtual ~CCurveEditorSelectionControllerListenerBase() override = default;

    virtual void OnAddedToSelection(const CurveEditorControllerSelection& selection) override;
    virtual void OnRemovedFromSelection(const CurveEditorControllerSelection& selection, bool clear) override;
};


#endif //__CURVE_EDITOR_SELECTION_LISTENER_BASE_H__