#include "pch.h"
#include "CurveViewComponent.h"
#include "SplineController.h"
#include "CurveEditorView.h"
#include <ImGuiInterop.h>

using namespace ax::ImGuiInterop;

CCurveEditorCurveView::CCurveEditorCurveView(ICurveEditorView& editorView, size_t curveIndex) :
    CCurveEditorSplineViewComponentBase(editorView),
    m_CurveIndex(curveIndex)
{
}

std::optional<ax::cubic_bezier_t> CCurveEditorCurveView::GetControlPointsPositions() const noexcept
{
    const auto& controller = GetController();
    if (!controller)
        return std::nullopt;

    std::array<ax::pointf, 4> controlPoints;

    const auto visitationResult = controller->VisitCurvePoints(m_CurveIndex, [iterator = controlPoints.begin(), endIterator = controlPoints.end()](const auto& point) mutable
    {
        EDITOR_ASSERT(iterator != endIterator);
        if (iterator != endIterator)
            *(iterator++) = point;
    });

    EDITOR_ASSERT(visitationResult);
    if (!visitationResult)
        return std::nullopt;

    return ax::cubic_bezier_t{ controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3] };
}

std::optional<ax::cubic_bezier_t> CCurveEditorCurveView::GetEditorControlPointsPositions() const noexcept
{
    auto controlPoints = GetControlPointsPositions();
    if (!controlPoints)
        return std::nullopt;

    const auto& editorCanvas = GetEditorView().GetCanvas();

    const auto translatePoint = [&editorCanvas](auto& point)
    {
        point = editorCanvas.ToEditor(point);
    };

    const auto first = &controlPoints->p0;

    for (auto i = 0u; i < 4; ++i)
        translatePoint(*(first + i));;

    return controlPoints;
}

void CCurveEditorCurveView::OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller)
{
    const auto controlPoints = GetEditorControlPointsPositions();
    EDITOR_ASSERT(controlPoints);
    if (!controlPoints)
        return;

    const auto& editorStyle = GetEditorStyle();
    const auto splineThickness = editorStyle.SplineThickness;

    const auto curveColor = controller.GetColor();

    const auto getControlPoint = [&controlPoints](size_t index)
    {
        return to_imvec(*(&controlPoints->p0 + index));
    };

    drawList.AddBezierCurve(getControlPoint(0), getControlPoint(1), getControlPoint(2), getControlPoint(3), curveColor, splineThickness);
}

bool CCurveEditorCurveView::IsColliding(const ax::pointf& point, float extraThickness /*= 0.0f*/) const noexcept
{
    auto bounds = CalculateBounds();
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return false;

    if (extraThickness > 0.0f)
        bounds->expand(extraThickness);

    if (!bounds->contains(point))
        return false;

    const auto controlPoints = GetEditorControlPointsPositions();
    const auto localResult = ax::cubic_bezier_project_point(point, controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, 10000);

    return localResult.distance <= GetEditorStyle().SplineThickness + extraThickness;
}

bool CCurveEditorCurveView::IsColliding(const ax::rectf& rect, bool allowIntersect /*= true*/) const noexcept
{
    const auto bounds = CalculateBounds();
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return false;

    if (rect.contains(*bounds))
        return true;

    if (!allowIntersect || !rect.intersects(*bounds))
        return false;

    const auto controlPoints = GetEditorControlPointsPositions();

    const auto topLeft = rect.top_left();
    const auto topRight = rect.top_right();
    const auto bottomRight = rect.bottom_right();
    const auto bottomLeft = rect.bottom_left();

    ax::pointf points[3];

    if (cubic_bezier_line_intersect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, topLeft, topRight, points) > 0)
        return true;
    if (cubic_bezier_line_intersect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, topRight, bottomRight, points) > 0)
        return true;
    if (cubic_bezier_line_intersect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, bottomRight, bottomLeft, points) > 0)
        return true;
    if (cubic_bezier_line_intersect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, bottomLeft, topLeft, points) > 0)
        return true;

    return false;
}

std::optional<ax::rectf> CCurveEditorCurveView::CalculateBounds() const noexcept
{
    const auto controlPoints = GetEditorControlPointsPositions();
    if (!controlPoints)
        return std::nullopt;

    auto result = ax::cubic_bezier_bounding_rect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3);

    if (result.w == 0.0f)
    {
        result.x -= 0.5f;
        result.w = 1.0f;
    }

    if (result.h == 0.0f)
    {
        result.y -= 0.5f;
        result.h = 1.0f;
    }

    result.expand(GetEditorStyle().SplineThickness);

    return result;
}

ECurveEditorSplineComponentType CCurveEditorCurveView::GetType() const noexcept
{
    return ECurveEditorSplineComponentType::Curve;
}