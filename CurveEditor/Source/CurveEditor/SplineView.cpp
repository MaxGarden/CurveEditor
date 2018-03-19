#include "pch.h"
#include "SplineView.h"
#include "SplineDataModel.h"
#include "SplineController.h"
#include <imgui/imgui.h>

CCurveEditorSplineView::CCurveEditorSplineView(CCurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

void CCurveEditorSplineView::OnFrame()
{
    if(m_Controller)
        ImGui::Text("Spline name: %s", m_Controller->GetName().c_str());
}

bool CCurveEditorSplineView::SetController(const IEditorControllerSharedPtr& controller) noexcept
{
    if(!controller)
    {
        m_Controller.reset();
        return true;
    }

    const auto splineEditorContoller = std::dynamic_pointer_cast<ICurveEditorSplineController>(controller);
    if (!splineEditorContoller)
        return false;

    m_Controller = std::move(splineEditorContoller);
    return true;
}