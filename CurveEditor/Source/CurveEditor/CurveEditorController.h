#pragma  once
#if !defined(__CURVE_EDITOR_CONTROLLER_H__)

#include "EditorListener.h"
#include "CurveEditorDataModel.h"
#include "EditorControllerBase.h"
#include "Style.h"

class ICurveEditorControllerListener : public IEditorListener
{
public:
    virtual ~ICurveEditorControllerListener() = default;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
};

class ICurveEditorController : public IEditorController
{
public:
    virtual ~ICurveEditorController() override = default;

    virtual bool SetActiveTool(ICurveEditorToolSharedPtr&& tool) noexcept = 0;
    virtual const ICurveEditorToolSharedPtr& GetActiveTool() const noexcept = 0;

    virtual const ICurveEditorSplineControllerSharedPtr& GetSplineController(const SplineID& id) const noexcept = 0;
    virtual void VisitSplineControllers(const ConstVisitorType<ICurveEditorSplineControllerSharedPtr>& visitor) const noexcept = 0;

    virtual const SCurveEditorStyle& GetEditorStyle() const noexcept = 0;

    static ICurveEditorControllerUniquePtr Create(ICurveEditorSplineControllerFactory& splineControllerFactory);
};

#endif //__CURVE_EDITOR_CONTROLLER_H__