#pragma  once
#if !defined(__CURVE_EDITOR_LISTENER_BASE_H__)

#include "CurveEditorController.h"

class CCurveEditorDataModelListenerBase : public ICurveEditorDataModelListener
{
public:
    CCurveEditorDataModelListenerBase() = default;
    virtual ~CCurveEditorDataModelListenerBase() override = default;

    virtual void OnStyleChanged(const SCurveEditorStyle& style) override;

    virtual void OnSplineCreated(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) override;
    virtual void OnSplineDestroyed(const ICurveEditorSplineDataModelSharedPtr& splineDataModel) override;

    virtual void OnSelectionDataModelChanged(const ICurveEditorSelectionDataModelSharedPtr& selectionDataModel) override;
};

class CCurveEditorControllerListenerBase : public ICurveEditorControllerListener
{
public:
    CCurveEditorControllerListenerBase() = default;
    virtual ~CCurveEditorControllerListenerBase() override = default;

    virtual void OnToolChanged(ICurveEditorTool* tool) override;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) override;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) override;
};

#endif //__CURVE_EDITOR_LISTENER_BASE_H__