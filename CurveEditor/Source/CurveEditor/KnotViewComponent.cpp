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

std::optional<ax::pointf> CCurveEditorKnotView::GetEditorPosition(bool screenTranslation) const noexcept
{
    const auto position = GetPosition();
    if (!position)
        return std::nullopt;

    const auto& editorCanvas = GetEditorView().GetCanvas();
    return editorCanvas.ToEditor(*position, screenTranslation);
}

void CCurveEditorKnotView::OnFrame(ImDrawList& drawList, ICurveEditorSplineController&)
{
    const auto bounds = CalculateBounds(true);
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return;

    const auto& editorStyle = GetEditorStyle();
    const auto& fillColor = editorStyle.Colors[CurveEditorStyleColor_Knot];

    drawList.AddRectFilled(to_imvec(bounds->top_left()), to_imvec(bounds->bottom_right()), fillColor);
}

std::optional<ax::rectf> CCurveEditorKnotView::CalculateBounds(bool screenTranslation) const noexcept
{
    const auto editorPosition = GetEditorPosition(screenTranslation);
    if (!editorPosition)
        return std::nullopt;

    auto halfKnotSize = to_pointf(GetEditorStyle().KnotSize) * 0.5;

    if (!screenTranslation)
    {
        const auto& windowCanvas = GetEditorView().GetCanvas().GetWindowCanvas();
        halfKnotSize = halfKnotSize.cwise_product(windowCanvas.GetInvertZoom());
    }

    return ax::rectf{ *editorPosition - halfKnotSize, *editorPosition + halfKnotSize };
}

bool CCurveEditorKnotView::IsColliding(const ax::pointf& position, float extraThickness /*= 0.0f*/) const noexcept
{
    auto bounds = CalculateBounds(false);
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return false;

    if (extraThickness > 0)
        bounds->expand(extraThickness);

    return bounds->contains(position);
}

bool CCurveEditorKnotView::IsColliding(const ax::rectf& rect, bool allowIntersect /*= true*/) const noexcept
{
    const auto bounds = CalculateBounds(false);
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return false;

    return !bounds->is_empty() && (allowIntersect ? bounds->intersects(rect) : rect.contains(*bounds));
}

ECurveEditorSplineComponentType CCurveEditorKnotView::GetType() const noexcept
{
    return ECurveEditorSplineComponentType::Knot;
}