#pragma  once
#if !defined(__CURVE_EDITOR_LISTENER_BASE_H__)

#include "CurveEditorController.h"

class CCurveEditorListenerBase : public ICurveEditorListener
{
public:
    CCurveEditorListenerBase() = default;
    virtual ~CCurveEditorListenerBase() override = default;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) override;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) override;
};

#endif //__CURVE_EDITOR_LISTENER_BASE_H__