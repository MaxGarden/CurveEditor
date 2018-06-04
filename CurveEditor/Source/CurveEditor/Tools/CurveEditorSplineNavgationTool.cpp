#include "pch.h"
#include "CurveEditorSplineNavgationTool.h"
#include "Components/NavigationComponent.h"
#include "Components/SplinesComponent.h"
#include "SplineView.h"

CCurveEditorSplineNavigationTool::CCurveEditorSplineNavigationTool(ECurveEditorMouseButton activationMouseButton) :
    m_ActivationMouseButton(activationMouseButton)
{
}

void CCurveEditorSplineNavigationTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    m_NavigationComponent = GetViewComponent<ICurveEditorNavigationComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_NavigationComponent.expired());

    m_SplinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_SplinesViewComponent.expired());
}

void CCurveEditorSplineNavigationTool::OnReleased(const CCurveEditorToolEvent&)
{
    m_NavigationComponent.reset();
    m_SplinesViewComponent.reset();
}

void CCurveEditorSplineNavigationTool::OnClickUp(const CCurveEditorToolMouseButtonEvent& event)
{
    if (m_ActivationMouseButton != event.GetMouseButton())
        return;

    const auto splinesViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splinesViewComponent);
    if (!splinesViewComponent)
        return;

    const auto splineComponent = splinesViewComponent->GetSplineComponentAt(event.GetMousePosition());
    if (!splineComponent)
        return;

    const auto splineView = splinesViewComponent->GetSplineView(splineComponent->GetSplineID());
    EDITOR_ASSERT(splineView);
    if (!splineView)
        return;

    const auto navigationComponent = m_NavigationComponent.lock();
    EDITOR_ASSERT(navigationComponent);
    if (!navigationComponent)
        return;

    ax::rectf splineBounds;

    const auto boundsVisitor = [&splineBounds](const auto& splineComponent)
    {
        const auto bounds = splineComponent.CalculateBounds(false);
        EDITOR_ASSERT(bounds);
        if (!bounds)
            return true;

        splineBounds = ax::make_union(splineBounds, *bounds);
        return true;
    };

    splineView->VisitSplineComponents(ECurveEditorSplineComponentType::Knot, boundsVisitor);
    splineView->VisitSplineComponents(ECurveEditorSplineComponentType::Tangent, boundsVisitor);

    navigationComponent->NavigateTo(splineBounds);
}