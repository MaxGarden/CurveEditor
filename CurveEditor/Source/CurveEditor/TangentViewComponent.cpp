#include "pch.h"
#include "TangentViewComponent.h"
#include "CurveViewComponent.h"
#include "CurveEditorView.h"
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

    const auto tangentPosition = GetEditorTangentPosition(true);
    EDITOR_ASSERT(tangentPosition);
    if (!tangentPosition)
        return;

    const auto imGuiTangentPosition = to_imvec(*tangentPosition);

    if (const auto anchorPosition = GetEditorAnchorPointPosition(true))
        drawList.AddLine(to_imvec(*anchorPosition), imGuiTangentPosition, style.Colors[CurveEditorStyleColor_TangentLine], style.TangentLineThickness);

    drawList.AddCircleFilled(imGuiTangentPosition, style.TangentMarkerRadius, style.Colors[CurveEditorStyleColor_TangentMarker]);
}

bool CCurveEditorTangentView::IsColliding(const ax::pointf& position, float extraThickness /*= 0.0f*/) const noexcept
{
    const auto tangentPosition = GetEditorTangentPosition(false);
    EDITOR_ASSERT(tangentPosition);
    if (!tangentPosition)
        return false;

    const auto& windowCanvas = GetEditorView().GetCanvas().GetWindowCanvas();

    const auto distance = (*tangentPosition - position).cwise_product(windowCanvas.GetZoom());
    const auto tangentRadius = GetEditorStyle().TangentMarkerRadius + extraThickness;

    return distance.length_sq() <= tangentRadius * tangentRadius;
}

bool CCurveEditorTangentView::IsColliding(const ax::rectf& rect, bool allowIntersect /*= true*/) const noexcept
{
    if (allowIntersect)
    {
        const auto tangentPosition = GetEditorTangentPosition(false);
        EDITOR_ASSERT(tangentPosition);
        if (!tangentPosition)
            return false;

        const auto tangentRadius = GetEditorStyle().TangentMarkerRadius;

        const auto& windowCanvas = GetEditorView().GetCanvas().GetWindowCanvas();
        const auto zoom = windowCanvas.GetZoom();

        const auto distance = (*tangentPosition - rect.center()).cwise_abs().cwise_product(zoom);

        const auto halfWidth = rect.w / 2.0f * zoom.x;
        const auto halfHeight = rect.h / 2.0f * zoom.y;

        if (distance.x > (halfWidth + tangentRadius) || distance.y > (halfHeight + tangentRadius))
            return false;

        if (distance.x <= halfWidth || distance.y <= halfHeight)
            return true;

        const auto delta = distance - ax::pointf{ halfWidth, halfHeight };

        return delta.length_sq() <= tangentRadius * tangentRadius;
    }
    else
    {
        const auto bounds = CalculateBounds(false);
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

std::optional<ax::pointf> CCurveEditorTangentView::GetEditorAnchorPointPosition(bool screenTranslation) const noexcept
{
    return ToEditorPosition(GetAnchorPointPosition(), screenTranslation);
}

std::optional<ax::pointf> CCurveEditorTangentView::GetTangentPosition() const noexcept
{
    if (const auto& controller = GetController())
        return controller->GetTangent(m_TangentIndex);

    return std::nullopt;
}

std::optional<ax::pointf> CCurveEditorTangentView::GetEditorTangentPosition(bool screenTranslation) const noexcept
{
    return ToEditorPosition(GetTangentPosition(), screenTranslation);
}

std::optional<ax::rectf> CCurveEditorTangentView::CalculateBounds(bool screenTranslation) const noexcept
{
    const auto tangentPosition = GetEditorTangentPosition(screenTranslation);
    EDITOR_ASSERT(tangentPosition);
    if (!tangentPosition)
        return std::nullopt;

    const auto tangentRadius = GetEditorStyle().TangentMarkerRadius;

    auto tangentRadiusBounds = ax::pointf{ tangentRadius, tangentRadius };

    if (!screenTranslation)
    {
        const auto& windowCanvas = GetEditorView().GetCanvas().GetWindowCanvas();
        tangentRadiusBounds = tangentRadiusBounds.cwise_product(windowCanvas.GetInvertZoom());
    }

    return ax::rectf{ tangentPosition->x - tangentRadiusBounds.x, tangentPosition->y - tangentRadiusBounds.x, 2 * tangentRadiusBounds.x, 2 * tangentRadiusBounds.y };
}

std::optional<ax::pointf> CCurveEditorTangentView::ToEditorPosition(const std::optional<ax::pointf>& position, bool screenTranslation) const noexcept
{
    if (!position)
        return std::nullopt;

    const auto& editorCanvas = GetEditorView().GetCanvas();
    return editorCanvas.ToEditor(*position, screenTranslation);
}