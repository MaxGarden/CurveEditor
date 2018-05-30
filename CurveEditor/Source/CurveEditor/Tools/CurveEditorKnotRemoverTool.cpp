#include "pch.h"
#include "CurveEditorKnotRemoverTool.h"
#include "Components/SplinesComponent.h"
#include "KnotViewComponent.h"

CCurveEditorKnotRemoverTool::CCurveEditorKnotRemoverTool(ECurveEditorMouseButton removeButton /* = ECurveEditorMouseButton::Left */) :
    m_RemoveButton(removeButton)
{
}

void CCurveEditorKnotRemoverTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    m_SpinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_SpinesViewComponent.expired());
}

void CCurveEditorKnotRemoverTool::OnReleased(const CCurveEditorToolEvent&)
{
    m_SpinesViewComponent.reset();
}

void CCurveEditorKnotRemoverTool::OnClickUp(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != m_RemoveButton)
        return;

    const auto splinesViewComponent = m_SpinesViewComponent.lock();
    EDITOR_ASSERT(splinesViewComponent);
    if (!splinesViewComponent)
        return;

    const auto splineViewComponent = splinesViewComponent->GetSplineComponentAt(event.GetMousePosition());
    if (!splineViewComponent)
        return;

    const auto knotView = GetKnotViewAtPosition(event.GetMousePosition());
    if (!knotView)
        return;

    const auto result = knotView->Remove();
    EDITOR_ASSERT(result);
}

ICurveEditorKnotView * CCurveEditorKnotRemoverTool::GetKnotViewAtPosition(const ax::pointf& position) const noexcept
{
    const auto splinesViewComponent = m_SpinesViewComponent.lock();
    EDITOR_ASSERT(splinesViewComponent);
    if (!splinesViewComponent)
        return nullptr;

    const auto splineViewComponent = splinesViewComponent->GetSplineComponentAt(position);
    if (!splineViewComponent || splineViewComponent->GetType() != ECurveEditorSplineComponentType::Knot)
        return nullptr;

    const auto result = dynamic_cast<ICurveEditorKnotView*>(splineViewComponent);
    EDITOR_ASSERT(result);

    return result;
}

