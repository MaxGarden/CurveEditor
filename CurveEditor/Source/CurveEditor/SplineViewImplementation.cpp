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

    static const auto onFrameVisitor = [](auto& view)
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
    m_TangentsViews.clear();
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
        auto view = ViewType::Create(editorView, index++);

        auto isValid = true;
        isValid &= view->SetController(controller);

        EDITOR_ASSERT(isValid);

        return isValid ? std::move(view) : nullptr;
    });
}

void CCurveEditorSplineView::EnsureCurvesViews(ICurveEditorSplineController& controller)
{
    EnsureViews<ICurveEditorCurveView>(m_CurvesViews, GetController(), m_EditorView, controller.GetCurvesCount());
}

void CCurveEditorSplineView::EnsureKnotsViews(ICurveEditorSplineController& controller)
{
    EnsureViews<ICurveEditorKnotView>(m_KnotsViews, GetController(), m_EditorView, controller.GetKnotsCount());
}

void CCurveEditorSplineView::EnsureTangentsViews(ICurveEditorSplineController& controller)
{
    EnsureViews<ICurveEditorTangentView>(m_TangentsViews, GetController(), m_EditorView, controller.GetTangentsCount());
}

void CCurveEditorSplineView::VisitCurvesViews(const InterruptibleVisitorType<ICurveEditorCurveView>& visitor, bool reverse /*= false*/) const noexcept
{
    VisitPointersContainer(m_CurvesViews, visitor, reverse);
}

void CCurveEditorSplineView::VisitKnotsViews(const InterruptibleVisitorType<ICurveEditorKnotView>& visitor, bool reverse /*= false*/) const noexcept
{
    VisitPointersContainer(m_KnotsViews, visitor, reverse);
}

void CCurveEditorSplineView::VisitTangentsViews(const InterruptibleVisitorType<ICurveEditorTangentView>& visitor, bool reverse /*= false*/) const noexcept
{
    VisitPointersContainer(m_TangentsViews, visitor, reverse);
}

void CCurveEditorSplineView::VisitSplineComponents(ECurveEditorSplineComponentType componentType, const InterruptibleVisitorType<ICurveEditorSplineComponentView>& visitor, bool reverse /*= false*/) const noexcept
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