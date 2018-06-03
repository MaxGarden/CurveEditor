#include "pch.h"
#include "CurveEditorRectNavigationTool.h"
#include "Components/NavigationComponent.h"

CCurveEditorRectNavigationTool::CCurveEditorRectNavigationTool(ECurveEditorMouseButton activationMouseButton) :
    CCurveEditorSelectionToolBase(activationMouseButton)
{
}

void CCurveEditorRectNavigationTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    m_NavigationComponent = GetViewComponent<ICurveEditorNavigationComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_NavigationComponent.expired());
}

void CCurveEditorRectNavigationTool::OnReleased(const CCurveEditorToolEvent&)
{
    m_NavigationComponent.reset();
}

void CCurveEditorRectNavigationTool::OnSelectionEnd(ICurveEditorView&)
{
    const auto navigationComponent = m_NavigationComponent.lock();
    EDITOR_ASSERT(navigationComponent);
    if (!navigationComponent)
        return;

    const auto selectionRect = GetSelectionRect();
    EDITOR_ASSERT(selectionRect);
    if (!selectionRect)
        return;

    navigationComponent->NavigateTo(*selectionRect);
}