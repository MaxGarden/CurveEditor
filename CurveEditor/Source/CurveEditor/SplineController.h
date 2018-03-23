#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_CONTROLLER__)

#include "EditorProtocol.h"
#include "EditorController.h"
#include "ax/ax.h"

class ICurveEditorSplineProtocol : public IEditorProtocol
{
public:
    virtual ~ICurveEditorSplineProtocol() override = default;
};

class ICurveEditorSplineController : public IEditorController
{
public:
    virtual ~ICurveEditorSplineController() = default;

    virtual const std::string& GetName() const noexcept = 0;
};

class ICurveEditorSplineControllerFactory
{
public:
    virtual ~ICurveEditorSplineControllerFactory() = default;

    virtual ICurveEditorSplineControllerUniquePtr Create(const ICurveEditorSplineDataModelSharedPtr& dataModel) = 0;
};

#endif //__CURVE_EDITOR_SPLINE_CONTROLLER__