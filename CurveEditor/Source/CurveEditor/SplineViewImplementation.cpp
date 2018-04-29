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
        return true;
    };

    VisitCurvesViews(onFrameVisitor);
    VisitTangentsViews(onFrameVisitor);
    VisitKnotsViews(onFrameVisitor);
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

void CCurveEditorSplineView::VisitCurvesViews(const InterruptibleVisitorType<CCurveEditorCurveView>& visitor, bool reverse /*= false*/) const noexcept
{
    VisitPointersContainer(m_CurvesViews, visitor, reverse);
}

void CCurveEditorSplineView::VisitKnotsViews(const InterruptibleVisitorType<CCurveEditorKnotView>& visitor, bool reverse /*= false*/) const noexcept
{
    VisitPointersContainer(m_KnotsViews, visitor, reverse);
}

void CCurveEditorSplineView::VisitTangentsViews(const InterruptibleVisitorType<CCurveEditorTangentView>& visitor, bool reverse /*= false*/) const noexcept
{
    VisitPointersContainer(m_TangentsViews, visitor, reverse);
}

void CCurveEditorSplineView::VisitSplineComponents(ECurveEditorSplineComponentType componentType, const InterruptibleVisitorType<ICurveEditorSplineViewComponent>& visitor, bool reverse /*= false*/) const noexcept
{
    switch (componentType)
    {
    case ECurveEditorSplineComponentType::Knot:
        VisitKnotsViews(visitor, reverse);
        break;
    case ECurveEditorSplineComponentType::Curve:
        VisitCurvesViews(visitor, reverse);
        break;
    case ECurveEditorSplineComponentType::Tangent:
        VisitTangentsViews(visitor, reverse);
        break;
    default:
        EDITOR_ASSERT(false && "Unsupported component type");
        break;
    }
}

ICurveEditorSplineViewUniquePtr CCurveEditorSplineViewFactory::Create(ICurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr&)
{
    return std::make_unique<CCurveEditorSplineView>(editorView);
}