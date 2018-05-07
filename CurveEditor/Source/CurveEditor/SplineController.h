#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_CONTROLLER__)

#include "ax/ax.h"
#include "EditorListener.h"
#include "EditorController.h"
#include "SplineDataModel.h"

class ICurveEditorSplineListener : public IEditorListener
{
public:
    virtual ~ICurveEditorSplineListener() override = default;

    virtual void OnKnotCreated(const ICurveEditorKnotControllerSharedPtr& knotController) = 0;
    virtual void OnKnotDestroyed(const ICurveEditorKnotControllerSharedPtr& knotController) = 0;

    virtual void OnTangentCreated(const ICurveEditorTangentControllerSharedPtr& tangentController) = 0;
    virtual void OnTangentDestroyed(const ICurveEditorTangentControllerSharedPtr& tangentController) = 0;

    virtual void OnCurveCreated(const ICurveEditorCurveControllerSharedPtr& curveController) = 0;
    virtual void OnCurveDestroyed(const ICurveEditorCurveControllerSharedPtr& curveController) = 0;
};

class ICurveEditorSplineController : public IEditorController
{
public:
    using CurveConstVisitor = std::function<void(const ax::pointf&)>;

public:
    virtual ~ICurveEditorSplineController() override = default;

    virtual const SplineID& GetID() const noexcept = 0;
    virtual const SplineColor& GetColor() const noexcept = 0;

    virtual void VisitKnotsControllers(const VisitorCopyType<ICurveEditorKnotControllerSharedPtr>& visitor) const = 0;
    virtual void VisitTangentsControllers(const VisitorCopyType<ICurveEditorTangentControllerSharedPtr>& visitor) const = 0;
    virtual void VisitCurvesControllers(const VisitorCopyType<ICurveEditorCurveControllerSharedPtr>& visitor) const = 0;

    static size_t ControlPointsPerCurve() noexcept;
};

class ICurveEditorSplineControllerFactory
{
public:
    virtual ~ICurveEditorSplineControllerFactory() = default;

    virtual ICurveEditorSplineControllerUniquePtr Create(const ICurveEditorSplineDataModelSharedPtr& dataModel) = 0;
};

#endif //__CURVE_EDITOR_SPLINE_CONTROLLER__