#include "pch.h"
#include "KnotViewComponent.h"
#include "CurveEditorView.h"
#include "SplineController.h"
#include <ImGuiInterop.h>

using namespace ax::ImGuiInterop;

CCurveEditorKnotView::CCurveEditorKnotView(ICurveEditorView& editorView, size_t knotIndex) :
    CCurveEditorSplineViewComponentBase(editorView),
    m_KnotIndex(knotIndex)
{
}

std::optional<ax::pointf> CCurveEditorKnotView::GetPosition() const noexcept
{
    if (const auto& controller = GetController())
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
    const auto bounds = CalculateBounds();
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return;

    const auto& editorStyle = GetEditorStyle();
    const auto& fillColor = editorStyle.Colors[CurveEditorStyleColor_Knot];

    drawList.AddRectFilled(to_imvec(bounds->top_left()), to_imvec(bounds->bottom_right()), fillColor);
}

std::optional<ax::rectf> CCurveEditorKnotView::CalculateBounds() const noexcept
{
    const auto editorPosition = GetEditorPosition();
    if (!editorPosition)
        return std::nullopt;

    const auto halfKnotSize = to_pointf(GetEditorStyle().KnotSize) * 0.5;
    return ax::rectf{ *editorPosition - halfKnotSize, *editorPosition + halfKnotSize };
}

bool CCurveEditorKnotView::IsColliding(const ax::pointf& point, float extraThickness /*= 0.0f*/) const noexcept
{
    auto bounds = CalculateBounds();
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return false;

    if (extraThickness > 0)
        bounds->expand(extraThickness);

    return bounds->contains(point);
}

bool CCurveEditorKnotView::IsColliding(const ax::rectf& rect, bool allowIntersect /*= true*/) const noexcept
{
    const auto bounds = CalculateBounds();
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return false;

    return !bounds->is_empty() && (allowIntersect ? bounds->intersects(rect) : rect.contains(*bounds));
}

ECurveEditorSplineComponentType CCurveEditorKnotView::GetType() const noexcept
{
    return ECurveEditorSplineComponentType::Knot;
}