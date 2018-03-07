#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_H__)

#include "EditorView.h"

class CCurveEditorSplineView final : public IEditorView
{
public:
    CCurveEditorSplineView() = default;
    virtual ~CCurveEditorSplineView() override final = default;

    virtual void OnFrame() override final;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept override final;
    virtual bool SetDataModel(const IEditorDataModelConstSharedPtr& dataModel) noexcept override final;

private:
    CCurveEditorSplineDataModelConstSharedPtr m_DataModel;
    CCurveEditorSplineControllerSharedPtr m_Controller;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_H__