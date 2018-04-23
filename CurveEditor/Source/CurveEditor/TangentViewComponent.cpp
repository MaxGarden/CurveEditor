#include "pch.h"
#include "TangentViewComponent.h"
#include "CurveViewComponent.h"
#include <ImGuiInterop.h>

using namespace ax::ImGuiInterop;

CCurveEditorTangentView::CCurveEditorTangentView(ICurveEditorView& editorView, size_t tangentPointIndex) :
    CCurveEditorSplineViewComponentBase(editorView),
    m_AnchorKnotIndex((tangentPointIndex == 0) ? 0 : (tangentPointIndex + 1) / 2),
    m_TangentIndex(tangentPointIndex)
{
}

void CCurveEditorTangentView::OnFrame(ImDrawList& drawList, ICurveEditorSplineController&)
{
    const auto& style = GetEditorStyle();

    const auto tangentPosition = GetEditorTangentPosition();
    EDITOR_ASSERT(tangentPosition);
    if (!tangentPosition)
        return;

    const auto imGuiTangentPosition = to_imvec(*tangentPosition);

    if (const auto anchorPosition = GetEditorAnchorPointPosition())
        drawList.AddLine(to_imvec(*anchorPosition), imGuiTangentPosition, style.Colors[CurveEditorStyleColor_TangentLine], style.TangentLineThickness);

    drawList.AddCircleFilled(imGuiTangentPosition, style.TangentMarkerRadius, style.Colors[CurveEditorStyleColor_TangentMarker]);
}

bool CCurveEditorTangentView::IsColliding(const ax::pointf& point, float extraThickness /*= 0.0f*/) const noexcept
{
    const auto tangentPosition = GetEditorTangentPosition();
    EDITOR_ASSERT(tangentPosition);
    if (!tangentPosition)
        return false;

    const auto distance = *tangentPosition - point;
    const auto tangentRadius = GetEditorStyle().TangentMarkerRadius + extraThickness;

    return distance.x * distance.x + distance.y * distance.y <= tangentRadius * tangentRadius;
}

bool CCurveEditorTangentView::IsColliding(const ax::rectf& rect, bool allowIntersect /*= true*/) const noexcept
{
    const auto tangentRadius = GetEditorStyle().TangentMarkerRadius;

    if (allowIntersect)
    {
        const auto tangentPosition = GetEditorTangentPosition();
        EDITOR_ASSERT(tangentPosition);
        if (!tangentPosition)
            return false;

        const auto distanceX = abs(tangentPosition->x - (rect.x + rect.w / 2.0f));
        const auto distanceY = abs(tangentPosition->y - (rect.y + rect.h / 2.0f));

        if (distanceX > (rect.w / 2.0f + tangentRadius))
            return false;

        if (distanceY > (rect.h / 2.0f + tangentRadius))
            return false;

        if (distanceX <= (rect.w / 2.0f))
            return true;

        if (distanceY <= (rect.h / 2.0f))
            return true;

        auto deltaX = (distanceX - rect.w / 2.0f);
        auto deltaY = (distanceY - rect.h / 2.0f);

        return deltaX * deltaX + deltaY * deltaY <= tangentRadius * tangentRadius;
    }
    else
    {
        const auto bounds = CalculateBounds();
        EDITOR_ASSERT(bounds);
        if (!bounds)
            return false;

        return rect.contains(*bounds);
    }
}

ECurveEditorSplineComponentType CCurveEditorTangentView::GetType() const noexcept
{
    return ECurveEditorSplineComponentType::Tangent;
}

static auto GetControlPointFromCurve(const std::optional<ax::cubic_bezier_t>& controlPoints, size_t index) -> std::optional<ax::pointf>
{
    if (!controlPoints)
        return std::nullopt;

    EDITOR_ASSERT(index < 4);
    if (index >= 4)
        return std::nullopt;

    return *(&controlPoints->p0 + index);
}

std::optional<ax::pointf> CCurveEditorTangentView::GetAnchorPointPosition() const noexcept
{
    if (const auto& controller = GetController())
        return controller->GetKnot(m_AnchorKnotIndex);

    return std::nullopt;
}

std::optional<ax::pointf> CCurveEditorTangentView::GetEditorAnchorPointPosition() const noexcept
{
    //TODO
    return std::nullopt;
}

std::optional<ax::pointf> CCurveEditorTangentView::GetTangentPosition() const noexcept
{
    if (const auto& controller = GetController())
        return controller->GetTangent(m_TangentIndex);

    return std::nullopt;
}

std::optional<ax::pointf> CCurveEditorTangentView::GetEditorTangentPosition() const noexcept
{
    //TODO
    return std::nullopt;
}

std::optional<ax::rectf> CCurveEditorTangentView::CalculateBounds() const noexcept
{
    const auto tangentPosition = GetEditorTangentPosition();
    EDITOR_ASSERT(tangentPosition);
    if (!tangentPosition)
        return std::nullopt;

    const auto tangentRadius = GetEditorStyle().TangentMarkerRadius;

    return ax::rectf{ tangentPosition->x - tangentRadius, tangentPosition->y - tangentRadius, 2 * tangentRadius, 2 * tangentRadius };
}