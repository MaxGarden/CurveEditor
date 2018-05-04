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
};

class ICurveEditorSplineController : public IEditorController
{
public:
    using CurveConstVisitor = std::function<void(const ax::pointf&)>;

public:
    virtual ~ICurveEditorSplineController() override = default;

    virtual const SplineID& GetID() const noexcept = 0;
    virtual const SplineColor& GetColor() const noexcept = 0;

    virtual const std::vector<ax::pointf>& GetControlPoints() const noexcept = 0;

    virtual bool VisitCurvePoints(size_t curveIndex, const CurveConstVisitor& visitor) const noexcept = 0;
    virtual size_t GetCurvesCount() const noexcept = 0;

    virtual std::optional<ax::pointf> GetKnot(size_t knotIndex) const noexcept = 0;
    virtual size_t GetKnotsCount() const noexcept = 0;

    virtual std::optional<ax::pointf> GetTangent(size_t tangentIndex) const noexcept = 0;
    virtual size_t GetTangentsCount() const noexcept = 0;
};

class ICurveEditorSplineControllerFactory
{
public:
    virtual ~ICurveEditorSplineControllerFactory() = default;

    virtual ICurveEditorSplineControllerUniquePtr Create(const ICurveEditorSplineDataModelSharedPtr& dataModel) = 0;
};

#endif //__CURVE_EDITOR_SPLINE_CONTROLLER__