#include "pch.h"
#include "SplineView.h"
#include "SplineDataModel.h"
#include "SplineController.h"
#include "CurveEditorView.h"
#include "Utilities.h"
#include <imgui/imgui.h>
#include <ImGuiInterop.h>

using namespace ax::ImGuiInterop;

void CCurveEditorSplineViewBase::OnFrame()
{
    const auto drawList = ImGui::GetWindowDrawList();
    EDITOR_ASSERT(drawList);
    if (!drawList)
        return;

    const auto controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return;

    OnFrame(*drawList, *controller);
}

void CCurveEditorSplineViewBase::OnFrame(ImDrawList&, ICurveEditorSplineController&)
{
    //to override
}

CCurveEditorView& CCurveEditorSplineViewBase::GetEditorView() noexcept
{
    return m_EditorView;
}

const CCurveEditorView& CCurveEditorSplineViewBase::GetEditorView() const noexcept
{
    return m_EditorView;
}

const SCurveEditorStyle& CCurveEditorSplineViewBase::GetEditorStyle() const noexcept
{
    static const auto null = SCurveEditorStyle{};

    if (const auto editorController = GetEditorView().GetController())
        editorController->GetEditorStyle();

    return null;;
}

CCurveEditorKnotView::CCurveEditorKnotView(CCurveEditorView& editorView, size_t knotIndex) :
    CCurveEditorSplineViewBase(editorView),
    m_KnotIndex(knotIndex)
{
}

void CCurveEditorKnotView::OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller)
{
    const auto bounds = GetBounds();
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return;

    const auto& editorStyle = GetEditorStyle();
    const auto& fillColor = editorStyle.Colors[CurveEditorStyleColor_Knot];

    drawList.AddRectFilled(to_imvec(bounds->top_left()), to_imvec(bounds->bottom_right()), fillColor);
}

std::optional<ax::pointf> CCurveEditorKnotView::GetPosition() const noexcept
{
    if (const auto controller = GetController())
        return controller->GetKnot(m_KnotIndex);

    return std::nullopt;
}

std::optional<ax::pointf> CCurveEditorKnotView::GetEditorPosition() const noexcept
{
    const auto position = GetPosition();
    if (!position)
        return std::nullopt;

    const auto& editorCanvas = GetEditorView().GetCanvas();
    return editorCanvas.ToEditor(*position);
}

std::optional<ax::rectf> CCurveEditorKnotView::GetBounds() const noexcept
{
    const auto editorPosition = GetEditorPosition();
    if (!editorPosition)
        return std::nullopt;

    const auto halfKnotSize = to_pointf(GetEditorStyle().KnotSize) * 0.5;
    return ax::rectf{ *editorPosition - halfKnotSize, *editorPosition + halfKnotSize };
}

CCurveEditorCurveView::CCurveEditorCurveView(CCurveEditorView& editorView, size_t curveIndex) :
    CCurveEditorSplineViewBase(editorView),
    m_CurveIndex(curveIndex)
{
}

void CCurveEditorCurveView::OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller)
{
    const auto controlPoints = GetEditorControlPointsPositions();
    EDITOR_ASSERT(controlPoints);
    if (!controlPoints)
        return;

    EDITOR_ASSERT(controlPoints->size() == 4);
    if (controlPoints->size() != 4)
        return;

    const auto& editorStyle = GetEditorStyle();
    const auto splineThickness = editorStyle.SplineThickness;

    const auto curveColor = controller.GetColor();

    const auto getControlPoint = [&controlPoints](size_t index)
    {
        return to_imvec(controlPoints->at(index));
    };

    drawList.AddBezierCurve(getControlPoint(0), getControlPoint(1), getControlPoint(2), getControlPoint(3), curveColor, splineThickness);
}

std::optional<CCurveEditorCurveView::ControlPoints> CCurveEditorCurveView::GetControlPointsPositions() const noexcept
{
    const auto controller = GetController();
    if (!controller)
        return std::nullopt;

    ControlPoints result;

    const auto visitationResult = controller->VisitCurvePoints(m_CurveIndex, [iterator = result.begin(), endIterator = result.end()](const auto& point) mutable
    {
        EDITOR_ASSERT(iterator != endIterator);
        if (iterator != endIterator)
            *(iterator++) = point;
    });

    EDITOR_ASSERT(visitationResult);
    if (!visitationResult)
        return std::nullopt;

    return result;
}

std::optional<CCurveEditorCurveView::ControlPoints> CCurveEditorCurveView::GetEditorControlPointsPositions() const noexcept
{
    auto controlPoints = GetControlPointsPositions();
    if (!controlPoints)
        return std::nullopt;

    const auto& editorCanvas = GetEditorView().GetCanvas();

    for (auto& controlPoint : *controlPoints)
        controlPoint = editorCanvas.ToEditor(controlPoint);

    return controlPoints;
}

CCurveEditorSplineView::CCurveEditorSplineView(CCurveEditorView& editorView) :
    CCurveEditorSplineViewBase(editorView)
{
}

void CCurveEditorSplineView::OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller)
{
    EnsureCurvesViews(controller);
    EnsureKnotsViews(controller);

    static const auto onFrameVisitor = [](CCurveEditorSplineViewBase& view)
    {
        view.OnFrame();
    };

    VisitCurveViews(onFrameVisitor);
    VisitKnotViews(onFrameVisitor);
}

void CCurveEditorSplineView::OnControllerChanged()
{
    m_CurvesViews.clear();
}

template<typename ViewType, typename ContainerType>
static void EnsureViews(ContainerType& container, const IEditorControllerSharedPtr& controller, CCurveEditorView& editorView, size_t count)
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
    EnsureViews<CCurveEditorCurveView>(m_CurvesViews, GetController(), GetEditorView(), controller.GetCurvesCount());
}

void CCurveEditorSplineView::EnsureKnotsViews(ICurveEditorSplineController& controller)
{
    EnsureViews<CCurveEditorKnotView>(m_KnotsViews, GetController(), GetEditorView(), controller.GetKnotsCount());
}

void CCurveEditorSplineView::VisitCurveViews(const std::function<void(CCurveEditorCurveView&)>& visitor) noexcept
{
    VisitContainer(m_CurvesViews, visitor);
}

void CCurveEditorSplineView::VisitKnotViews(const std::function<void(CCurveEditorKnotView&)>& visitor) noexcept
{
    VisitContainer(m_KnotsViews, visitor);
}

CCurveEditorSplineViewBase::CCurveEditorSplineViewBase(CCurveEditorView& editorView) :
    m_EditorView(editorView)
{
}