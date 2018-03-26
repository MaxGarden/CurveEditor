#pragma  once
#if !defined(__CURVE_EDITOR_PROTOCOL_BASE_H__)

#include "CurveEditorController.h"

class CCurveEditorProtocolBase : public ICurveEditorProtocol
{
public:
    CCurveEditorProtocolBase() = default;
    virtual ~CCurveEditorProtocolBase() override = default;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) override;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) override;
};

#endif //__CURVE_EDITOR_PROTOCOL_BASE_H__