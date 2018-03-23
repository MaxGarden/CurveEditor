#pragma  once
#if !defined(__CURVE_EDITOR_FUNCTION_SPLINE_CONTROLLER__)

#include "SplineDataModel.h"
#include "SplineController.h"
#include "EditorControllerBase.h"

class CCurveEditorFunctionSplineController final : public CEditorControllerBase<ICurveEditorSplineController, ICurveEditorSplineDataModel, ICurveEditorSplineProtocol>
{
public:
    CCurveEditorFunctionSplineController() = default;
    virtual ~CCurveEditorFunctionSplineController() override final = default;

    const std::string& GetName() const noexcept override final;

protected:
    virtual void OnDataModelChanged() override final;

private:
    void OnSplineModified() noexcept;
    void SortControlPoints(std::vector<ax::pointf>& controlPoints) noexcept;
};


#endif //__CURVE_EDITOR_FUNCTION_SPLINE_CONTROLLER__