#include "pch.h"
#include "CurveEditorHoveringTool.h"
#include "CurveEditorViewVisibleComponentBase.h"
#include "Components/SplinesComponent.h"

class CCurveEditorHoveringToolViewComponent final : public CCurveEditorViewVisibleComponentBase<>
{
public:
    CCurveEditorHoveringToolViewComponent(ICurveEditorView& editorView, IEditorRenderableUniquePtr&& borderRenderable);
    virtual ~CCurveEditorHoveringToolViewComponent() override final = default;

    virtual bool Initialize() override final;

    virtual void OnFrame() override final;

private:
    IEditorRenderableUniquePtr m_BorederRenderable;
};

CCurveEditorHoveringToolViewComponent::CCurveEditorHoveringToolViewComponent(ICurveEditorView& editorView, IEditorRenderableUniquePtr&& borderRenderable) :
    CCurveEditorViewVisibleComponentBase(editorView),
    m_BorederRenderable(std::move(borderRenderable))
{
}

bool CCurveEditorHoveringToolViewComponent::Initialize()
{
    return m_BorederRenderable != nullptr;
}

void CCurveEditorHoveringToolViewComponent::OnFrame()
{
    EDITOR_ASSERT(m_BorederRenderable);
    if (m_BorederRenderable)
        m_BorederRenderable->OnFrame();
}

void CCurveEditorHoveringTool::OnActiveEditorViewChanged(const CCurveEditorToolEvent& event)
{
    m_SplinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(event.GetEditorView());
}

void CCurveEditorHoveringTool::OnMouseMove(const CCurveEditorToolMouseEvent& event)
{
    const auto splineViews = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splineViews);
    if (!splineViews)
        return;

    const auto hoveredComponent = splineViews->GetSplineComponentAt(event.GetMousePosition());
    UpdateHoveringView(hoveredComponent, event.GetEditorView());
}

void CCurveEditorHoveringTool::UpdateHoveringView(ICurveEditorSplineViewComponent* splineViewComponent, ICurveEditorView& editorView)
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

    auto hoveringBorderRenerable = m_LastHoveredSplineViewComponent->CreateBorderRenderable(CurveEditorStyleColor_HoveringBorder, CurveEditorStyleFloat_HoveringBorderThickness);
    EDITOR_ASSERT(hoveringBorderRenerable);
    if (!hoveringBorderRenerable)
        return;

    auto hoveringToolViewComponent = std::make_unique<CCurveEditorHoveringToolViewComponent>(editorView, std::move(hoveringBorderRenerable));

    m_HoveringViewHandle = AddToolView(editorView, std::move(hoveringToolViewComponent), EComponentOrder::Foreground);
}