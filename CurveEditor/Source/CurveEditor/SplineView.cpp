#include "pch.h"
#include "SplineView.h"
#include "SplineDataModel.h"
#include "SplineController.h"
#include "CurveEditorView.h"
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

ICurveEditorView& CCurveEditorSplineViewBase::GetEditorView() noexcept
{
    return m_EditorView;
}

const ICurveEditorView& CCurveEditorSplineViewBase::GetEditorView() const noexcept
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

CCurveEditorKnotView::CCurveEditorKnotView(ICurveEditorView& editorView, size_t knotIndex) :
    CCurveEditorSplineViewBase(editorView),
    m_KnotIndex(knotIndex)
{
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

void CCurveEditorKnotView::OnFrame(ImDrawList& drawList, ICurveEditorSplineController&)
{
    const auto bounds = GetBounds();
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return;

    const auto& editorStyle = GetEditorStyle();
    const auto& fillColor = editorStyle.Colors[CurveEditorStyleColor_Knot];

    drawList.AddRectFilled(to_imvec(bounds->top_left()), to_imvec(bounds->bottom_right()), fillColor);
}

std::optional<ax::rectf> CCurveEditorKnotView::GetBounds() const noexcept
{
    const auto editorPosition = GetEditorPosition();
    if (!editorPosition)
        return std::nullopt;

    const auto halfKnotSize = to_pointf(GetEditorStyle().KnotSize) * 0.5;
    return ax::rectf{ *editorPosition - halfKnotSize, *editorPosition + halfKnotSize };
}

CCurveEditorTangentView::CCurveEditorTangentView(ICurveEditorView& editorView, CCurveEditorCurveView& curveView, size_t anchorPointIndex, size_t tangentPointIndex) :
    CCurveEditorSplineViewBase(editorView),
    m_CurveView(curveView),
    m_AnchorControlPointIndex(anchorPointIndex),
    m_TangentControlPointIndex(tangentPointIndex)
{
}

void CCurveEditorTangentView::OnFrame(ImDrawList& drawList, ICurveEditorSplineController&)
{
    const auto controlPoints = m_CurveView.GetEditorControlPointsPositions();
    EDITOR_ASSERT(controlPoints);
    if (!controlPoints)
        return;

    EDITOR_ASSERT(m_AnchorControlPointIndex < controlPoints->size() && m_TangentControlPointIndex < controlPoints->size());
    if (m_TangentControlPointIndex >= controlPoints->size())
        return;

    const auto& style = GetEditorStyle();
    const auto tangentPosition = to_imvec(controlPoints->at(m_TangentControlPointIndex));

    if (m_AnchorControlPointIndex < controlPoints->size())
    {
        const auto anchorPosition = to_imvec(controlPoints->at(m_AnchorControlPointIndex));
        drawList.AddLine(anchorPosition, tangentPosition, style.Colors[CurveEditorStyleColor_TangentLine], style.TangentLineThickness);
    }

    drawList.AddCircleFilled(tangentPosition, style.TangentMarkerRadius, style.Colors[CurveEditorStyleColor_TangentMarker]);
}

CCurveEditorCurveView::CCurveEditorCurveView(ICurveEditorView& editorView, size_t curveIndex) :
    CCurveEditorSplineViewBase(editorView),
    m_CurveIndex(curveIndex)
{
    m_Tangents.reserve(2);
    m_Tangents.emplace_back(std::make_unique<CCurveEditorTangentView>(editorView, *this, 0u, 1u));
    m_Tangents.emplace_back(std::make_unique<CCurveEditorTangentView>(editorView, *this, 3u, 2u));
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

    VisitTangentsViews([](auto& tangentView)
    {
        tangentView.OnFrame();
    });
}

void CCurveEditorCurveView::OnControllerChanged()
{
    VisitTangentsViews([controller = GetController()](auto& tangentView)
    {
        tangentView.SetController(controller);
    });
}

void CCurveEditorCurveView::VisitTangentsViews(const VisitorType<CCurveEditorSplineViewBase>& visitor) noexcept
{
    VisitPointersContainer(m_Tangents, visitor);
}

CCurveEditorSplineView::CCurveEditorSplineView(ICurveEditorView& editorView) :
    CCurveEditorSplineViewBase(editorView)
{
}

void CCurveEditorSplineView::OnFrame(ImDrawList&, ICurveEditorSplineController& controller)
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
    EnsureViews<CCurveEditorCurveView>(m_CurvesViews, GetController(), GetEditorView(), controller.GetCurvesCount());
}

void CCurveEditorSplineView::EnsureKnotsViews(ICurveEditorSplineController& controller)
{
    EnsureViews<CCurveEditorKnotView>(m_KnotsViews, GetController(), GetEditorView(), controller.GetKnotsCount());
}

void CCurveEditorSplineView::VisitCurveViews(const VisitorType<CCurveEditorCurveView>& visitor) noexcept
{
    VisitPointersContainer(m_CurvesViews, visitor);
}

void CCurveEditorSplineView::VisitKnotViews(const VisitorType<CCurveEditorKnotView>& visitor) noexcept
{
    VisitPointersContainer(m_KnotsViews, visitor);
}

CCurveEditorSplineViewBase::CCurveEditorSplineViewBase(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
}