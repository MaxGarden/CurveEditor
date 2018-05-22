#include "pch.h"
#include "CurveEditorHoveringTool.h"
#include "CurveEditorViewVisibleComponentBase.h"
#include "Components/SplinesComponent.h"
#include "SplineView.h"

class CCurveEditorHoveringToolViewComponent final : public CCurveEditorViewVisibleComponentBase<>
{
public:
    CCurveEditorHoveringToolViewComponent(ICurveEditorView& editorView, std::vector<IEditorRenderableUniquePtr>&& borderRenderables);
    virtual ~CCurveEditorHoveringToolViewComponent() override final = default;

    virtual bool Initialize() override final;

    virtual void OnFrame() override final;

private:
    const std::vector<IEditorRenderableUniquePtr> m_BorderRenderables;
};

CCurveEditorHoveringToolViewComponent::CCurveEditorHoveringToolViewComponent(ICurveEditorView& editorView, std::vector<IEditorRenderableUniquePtr>&& borderRenderables) :
    CCurveEditorViewVisibleComponentBase(editorView),
    m_BorderRenderables(std::move(borderRenderables))
{
}

bool CCurveEditorHoveringToolViewComponent::Initialize()
{
    return !m_BorderRenderables.empty();
}

void CCurveEditorHoveringToolViewComponent::OnFrame()
{
    VisitPointersContainer(m_BorderRenderables, [](auto& borderRenderable)
    {
        borderRenderable.OnFrame();
    });
}

CCurveEditorHoveringTool::CCurveEditorHoveringTool(bool curveAsSpline) :
    m_CurveAsSpline(curveAsSpline)
{
}

void CCurveEditorHoveringTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    m_SplinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_SplinesViewComponent.expired());
}

void CCurveEditorHoveringTool::OnReleased(const CCurveEditorToolEvent& event)
{
    m_SplinesViewComponent.reset();
    UpdateHoveringView(nullptr, event.GetEditorView());
}

void CCurveEditorHoveringTool::OnDragBegin(const CCurveEditorToolMouseButtonEvent&)
{
    m_IsEnabled = false;
}

void CCurveEditorHoveringTool::OnDragEnd(const CCurveEditorToolMouseButtonEvent&)
{
    m_IsEnabled = true;
}

void CCurveEditorHoveringTool::OnMouseMove(const CCurveEditorToolMouseEvent& event)
{
    if (!m_IsEnabled)
        return;

    const auto splineViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViewComponent);
    if (!splineViewComponent)
        return;

    const auto hoveredComponent = splineViewComponent->GetSplineComponentAt(event.GetMousePosition());
    UpdateHoveringView(hoveredComponent, event.GetEditorView());
}

void CCurveEditorHoveringTool::UpdateHoveringView(ICurveEditorSplineComponentView* splineViewComponent, ICurveEditorView& editorView)
{
    if (splineViewComponent == m_LastHoveredSplineViewComponent)
        return;

    if (m_HoveringViewHandle)
    {
        RemoveToolView(*m_HoveringViewHandle);
        m_HoveringViewHandle.reset();
    }

    m_LastHoveredSplineViewComponent = splineViewComponent;
    if (!m_LastHoveredSplineViewComponent)
        return;

    auto borderRenderables = CreateBorderRenderables(*m_LastHoveredSplineViewComponent);
    if (borderRenderables.empty())
        return;

    auto hoveringToolViewComponent = std::make_unique<CCurveEditorHoveringToolViewComponent>(editorView, std::move(borderRenderables));
    m_HoveringViewHandle = AddToolView(editorView, std::move(hoveringToolViewComponent), EComponentOrder::Foreground);
}

std::vector<IEditorRenderableUniquePtr> CCurveEditorHoveringTool::CreateBorderRenderables(ICurveEditorSplineComponentView& splineComponentView) const
{
    std::vector<IEditorRenderableUniquePtr> result;

    const auto createBorderRenderable = [](const auto& splineComponentView)
    {
        return splineComponentView.CreateBorderRenderable(CurveEditorStyleColor_HoveringBorder, CurveEditorStyleFloat_HoveringBorderThickness);
    };

    if (m_CurveAsSpline && splineComponentView.GetType() == ECurveEditorSplineComponentType::Curve)
    {
        const auto splineViewComponent = m_SplinesViewComponent.lock();
        EDITOR_ASSERT(splineViewComponent);
        if (!splineViewComponent)
            return result;

        const auto splineView = splineViewComponent->GetSplineView(splineComponentView.GetSplineID());
        EDITOR_ASSERT(splineView);
        if (!splineView)
            return result;

        splineView->VisitSplineComponents(ECurveEditorSplineComponentType::Curve, [&result, &createBorderRenderable](const auto& splineComponentView)
        {
            result.emplace_back(createBorderRenderable(splineComponentView));
            return true;
        });
    }
    else
    {
        auto hoveringBorderRenerable = createBorderRenderable(splineComponentView);
        EDITOR_ASSERT(hoveringBorderRenerable);
        if (!hoveringBorderRenerable)
            return result;

        result.emplace_back(std::move(hoveringBorderRenerable));
    }

    return result;
}