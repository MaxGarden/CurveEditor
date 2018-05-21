#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_CONTROLLER__)

#include "ax/ax.h"
#include "EditorListener.h"
#include "EditorController.h"
#include "SplineDataModel.h"

enum class ECurveEditorSplineComponentType
{
    Knot,
    Curve,
    Tangent,
};

class ICurveEditorSplineComponentController : public IEditorController
{
public:
    virtual ~ICurveEditorSplineComponentController() override = default;

    virtual const SplineID& GetSplineID() const noexcept = 0;
    virtual ECurveEditorSplineComponentType GetType() const noexcept = 0;
    virtual std::optional<size_t> GetIndex() const noexcept = 0;

    virtual bool SetPosition(const ax::pointf& position) = 0;
    virtual std::optional<ax::pointf> GetPosition() const noexcept = 0;
};

class ICurveEditorSplineControllerListener : public IEditorListener
{
public:
    virtual ~ICurveEditorSplineControllerListener() override = default;

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

    virtual void VisitKnotsControllers(const ConstVisitorType<ICurveEditorKnotControllerSharedPtr>& visitor) const = 0;
    virtual void VisitTangentsControllers(const ConstVisitorType<ICurveEditorTangentControllerSharedPtr>& visitor) const = 0;
    virtual void VisitCurvesControllers(const ConstVisitorType<ICurveEditorCurveControllerSharedPtr>& visitor) const = 0;

    virtual ICurveEditorSplineComponentControllerSharedPtr GetSplineComponentController(ECurveEditorSplineComponentType type, size_t index) const noexcept = 0;

    static size_t ControlPointsPerCurve() noexcept;
};

class ICurveEditorSplineControllerFactory
{
public:
    virtual ~ICurveEditorSplineControllerFactory() = default;

    virtual ICurveEditorSplineControllerUniquePtr Create(const ICurveEditorSplineDataModelSharedPtr& dataModel) = 0;
};

#endif //__CURVE_EDITOR_SPLINE_CONTROLLER__