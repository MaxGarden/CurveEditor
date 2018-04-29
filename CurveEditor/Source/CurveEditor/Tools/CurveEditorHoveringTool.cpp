#include "pch.h"
#include "CurveEditorHoveringTool.h"
#include "Components/SplinesComponent.h"

void CCurveEditorHoveringTool::OnActiveEditorViewChanged(const CCurveEditorToolEvent& event)
{
    m_SplinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(event.GetEditorView());
}

void CCurveEditorHoveringTool::OnDragUpdate(const CCurveEditorToolMouseDragEvent& event)
{
    if (event.GetMouseButton() != ECurveEditorMouseButton::Left)
        return;

    const auto splineViews = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViews);
    if (!splineViews)
        return;

    size_t componentsCount = 0;
    const auto visitor = [&componentsCount](const auto&)
    {
        ++componentsCount;
    };

    splineViews->VisitSplineComponentsInRect(visitor, ax::rectf{ m_DragBeginPosition, event.GetMousePosition() });
    const auto& mousePosition = event.GetMousePosition();

    ImGui::Text("Drag begin: %f %f", m_DragBeginPosition.x, m_DragBeginPosition.y);
    ImGui::Text("Mouse: %f %f", mousePosition.x, mousePosition.y);
    ImGui::Text("Components %d", componentsCount);

    if (const auto component = splineViews->GetSplineComponentAt(event.GetMousePosition()))
    {
        if (component->GetType() == ECurveEditorSplineComponentType::Tangent)
            ImGui::Text("NAD TANGENTEM");
        else if (component->GetType() == ECurveEditorSplineComponentType::Knot)
            ImGui::Text("NAD WEZLEM");
        else if (component->GetType() == ECurveEditorSplineComponentType::Curve)
            ImGui::Text("NAD KRZYWA");
    }
}

void CCurveEditorHoveringTool::OnDragBegin(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != ECurveEditorMouseButton::Left)
        return;

    m_DragBeginPosition = event.GetMousePosition();
}
