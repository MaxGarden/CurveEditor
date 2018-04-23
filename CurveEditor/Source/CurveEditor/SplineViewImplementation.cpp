#include "pch.h"
#include "SplineViewImplementation.h"

CCurveEditorSplineView::CCurveEditorSplineView(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

void CCurveEditorSplineView::OnFrame()
{
    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return;

    OnFrame(*controller);
}

void CCurveEditorSplineView::OnFrame(ICurveEditorSplineController& controller)
{
    EnsureCurvesViews(controller);
    EnsureKnotsViews(controller);
    EnsureTangentsViews(controller);

    static const auto onFrameVisitor = [](CCurveEditorSplineViewComponentBase& view)
    {
        view.OnFrame();
    };

    VisitCurvesViews(onFrameVisitor);
    VisitKnotsViews(onFrameVisitor);
    VisitTangentsViews(onFrameVisitor);
}

void CCurveEditorSplineView::OnControllerChanged()
{
    m_CurvesViews.clear();
    m_KnotsViews.clear();
}

template<typename ViewType, typename ContainerType>
static void EnsureViews(ContainerType& container, const IEditorControllerSharedPtr& controller, ICurveEditorView& editorView, size_t count)
{
    if (count == container.size())
        return;

    const auto previousSize = container.size();
    container.resize(count);

    if (count < previousSize)
        return;

    std::generate(container.begin() + previousSize, container.end(), [index = previousSize, &controller, &editorView]() mutable
    {
        auto view = std::make_unique<ViewType>(editorView, index++);

        auto isValid = true;
        isValid &= view->SetController(controller);

        EDITOR_ASSERT(isValid);

        return isValid ? std::move(view) : nullptr;
    });
}

void CCurveEditorSplineView::EnsureCurvesViews(ICurveEditorSplineController& controller)
{
    EnsureViews<CCurveEditorCurveView>(m_CurvesViews, GetController(), m_EditorView, controller.GetCurvesCount());
}

void CCurveEditorSplineView::EnsureKnotsViews(ICurveEditorSplineController& controller)
{
    EnsureViews<CCurveEditorKnotView>(m_KnotsViews, GetController(), m_EditorView, controller.GetKnotsCount());
}

void CCurveEditorSplineView::EnsureTangentsViews(ICurveEditorSplineController& controller)
{
    EnsureViews<CCurveEditorTangentView>(m_TangentsViews, GetController(), m_EditorView, controller.GetTangentsCount());
}

void CCurveEditorSplineView::VisitCurvesViews(const VisitorType<CCurveEditorCurveView>& visitor) noexcept
{
    VisitPointersContainer(m_CurvesViews, visitor);
}

void CCurveEditorSplineView::VisitKnotsViews(const VisitorType<CCurveEditorKnotView>& visitor) noexcept
{
    VisitPointersContainer(m_KnotsViews, visitor);
}

void CCurveEditorSplineView::VisitTangentsViews(const VisitorType<CCurveEditorTangentView>& visitor) noexcept
{
    VisitPointersContainer(m_TangentsViews, visitor);
}

void CCurveEditorSplineView::VisitSplineComponents(const VisitorType<ICurveEditorSplineViewComponent>& visitor) const noexcept
{
    if (!visitor)
        return;


}

ICurveEditorSplineViewUniquePtr CCurveEditorSplineViewFactory::Create(ICurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr&)
{
    return std::make_unique<CCurveEditorSplineView>(editorView);
}