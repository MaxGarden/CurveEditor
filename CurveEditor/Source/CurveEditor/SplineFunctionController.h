#pragma  once
#if !defined(__CURVE_EDITOR_FUNCTION_SPLINE_CONTROLLER__)

#include "SplineController.h"

class CCurveEditorFunctionSplineController final : public ICurveEditorSplineController
{
public:
    CCurveEditorFunctionSplineController() = default;
    virtual ~CCurveEditorFunctionSplineController() override final = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) noexcept override final;

    const std::string& GetName() const noexcept override final;

private:
    void OnSplineModified() noexcept;
    void SortControlPoints(std::vector<ax::pointf>& controlPoints) noexcept;

private:
    ICurveEditorSplineDataModelSharedPtr m_DataModel;
};


#endif //__CURVE_EDITOR_FUNCTION_SPLINE_CONTROLLER__