#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_CONTROLLER__)

#include "EditorListener.h"
#include "EditorController.h"
#include "ax/ax.h"

class ICurveEditorSplineListener : public IEditorListener
{
public:
    virtual ~ICurveEditorSplineListener() override = default;
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