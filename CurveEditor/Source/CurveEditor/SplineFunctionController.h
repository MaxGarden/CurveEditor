#pragma  once
#if !defined(__CURVE_EDITOR_FUNCTION_SPLINE_CONTROLLER__)

#include "SplineDataModel.h"
#include "SplineController.h"
#include "EditorControllerBase.h"

class CCurveEditorFunctionSplineController final : public CEditorControllerBase<ICurveEditorSplineController, ICurveEditorSplineDataModel, ICurveEditorSplineListener>
{
public:
    CCurveEditorFunctionSplineController() = default;
    virtual ~CCurveEditorFunctionSplineController() override final = default;

    const std::string& GetName() const noexcept override final;
    const std::vector<ax::pointf>& GetControlPoints() const noexcept override final;

    virtual bool VisitCurvePoints(size_t curveIndex, const CurveConstVisitor& visitor) const noexcept override final;
    virtual size_t GetCurvesCount() const noexcept override final;

    virtual std::optional<ax::pointf> GetKnot(size_t knotIndex) const noexcept override final;
    virtual size_t GetKnotsCount() const noexcept override final;

protected:
    virtual void OnDataModelChanged() override final;

private:
    void OnSplineModified() noexcept;
    void SortControlPoints(std::vector<ax::pointf>& controlPoints) noexcept;
};


#endif //__CURVE_EDITOR_FUNCTION_SPLINE_CONTROLLER__