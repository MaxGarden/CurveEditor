#include "pch.h"
#include "SplineView.h"
#include "SplineDataModel.h"
#include "SplineController.h"

CCurveEditorSplineView::CCurveEditorSplineView(CCurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

void CCurveEditorSplineView::OnFrame()
{

}

bool CCurveEditorSplineView::SetController(const IEditorControllerSharedPtr& controller) noexcept
{
    if(!controller)
    {
        m_Controller.reset();
        return true;
    }

    const auto splineEditorContoller = std::dynamic_pointer_cast<CCurveEditorSplineController>(controller);
    if (!splineEditorContoller)
        return false;

    m_Controller = std::move(splineEditorContoller);
    return true;
}

bool CCurveEditorSplineView::SetDataModel(const IEditorDataModelConstSharedPtr& dataModel) noexcept
{
    if (!dataModel)
    {
        m_DataModel.reset();
        return true;
    }

    const auto splineDataModel = std::dynamic_pointer_cast<const CCurveEditorSplineDataModel>(dataModel);
    if (!splineDataModel)
        return false;

    m_DataModel = std::move(splineDataModel);
    return true;
}