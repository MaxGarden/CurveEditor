#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_CONTROLLER__)

#include "EditorController.h"
#include "ax/ax.h"

class CCurveEditorFunctionSplineController final : public IEditorController
{
public:
    CCurveEditorFunctionSplineController() = default;
    virtual ~CCurveEditorFunctionSplineController() override final = default;

    virtual bool SetDataModel(const IEditorDataModelSharedPtr& dataModel) noexcept override final;

private:
    void OnSplineModified() noexcept;
    void SortControlPoints(std::vector<ax::pointf>& controlPoints) noexcept;

private:
    CCurveEditorSplineDataModelSharedPtr m_DataModel;
};


#endif //__CURVE_EDITOR_SPLINE_CONTROLLER__