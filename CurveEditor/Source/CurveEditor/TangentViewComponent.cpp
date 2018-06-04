#include "pch.h"
#include "TangentViewComponent.h"
#include "CurveViewComponent.h"
#include "TangentController.h"
#include "CurveEditorView.h"
#include "EditorRenderableBase.h"
#include "SplineComponentViewBase.h"
#include <ImGuiInterop.h>

using namespace ax::ImGuiInterop;

DECLARE_POINTERS(CCurveEditorTangentView);

class CCurveEditorTangentBorderRenderable final : public CCurveEditorSplineComponentBorderRenderableBase<CCurveEditorTangentView>
{
public:
    CCurveEditorTangentBorderRenderable(CCurveEditorTangentViewConstWeakPtr&& tangentView, ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle);
    virtual ~CCurveEditorTangentBorderRenderable() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList) override final;
};

class CCurveEditorTangentView final : public CCurveEditorSplineComponentViewBase<ICurveEditorTangentView, ICurveEditorTangentController, ECurveEditorSplineComponentType::Tangent>, public std::enable_shared_from_this<CCurveEditorTangentView>
{
friend CCurveEditorTangentBorderRenderable;
public:
    CCurveEditorTangentView(ICurveEditorView& editorView);
    virtual ~CCurveEditorTangentView() override final = default;

    virtual bool IsColliding(const ax::pointf& position, float extraThickness = 0.0f) const noexcept override final;
    virtual bool IsColliding(const ax::rectf& rect, bool allowIntersect = true) const noexcept override final;

    virtual std::optional<ax::rectf> CalculateBounds(bool screenTranslation) const noexcept override final;

    virtual IEditorRenderableUniquePtr CreateBorderRenderable(ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) const override final;

protected:
    virtual void OnFrame(ImDrawList& drawList) override final;

private:
    std::optional<ax::pointf> GetAnchorPointPosition() const noexcept;
    std::optional<ax::pointf> GetEditorAnchorPointPosition(bool screenTranslation) const noexcept;

    std::optional<ax::pointf> GetTangentPosition() const noexcept;
    std::optional<ax::pointf> GetEditorTangentPosition(bool screenTranslation) const noexcept;

    std::optional<ax::pointf> ToEditorPosition(const std::optional<ax::pointf>& position, bool screenTranslation) const noexcept;
};

CCurveEditorTangentBorderRenderable::CCurveEditorTangentBorderRenderable(CCurveEditorTangentViewConstWeakPtr&& tangentView, ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) :
    CCurveEditorSplineComponentBorderRenderableBase(std::move(tangentView), borderStyleColor, thicknessStyle)
{
}

void CCurveEditorTangentBorderRenderable::OnFrame(ImDrawList& drawList)
{
    const auto tangentView = GetSplineComponentView().lock();
    if (!tangentView)
        return;

    const auto editorPosition = tangentView->GetEditorTangentPosition(true);
    EDITOR_ASSERT(editorPosition);
    if (!editorPosition)
        return;

    const auto& editorStyle = tangentView->GetEditorView().GetEditorStyle();

    const auto thickness = editorStyle.GetEditorStyleFloat(GetThichnessStyle());
    if (!thickness || thickness <= 0.0f)
        return;

    const auto& color = editorStyle.Colors[GetBorderStyleColor()];

    drawList.AddCircle(to_imvec(*editorPosition), editorStyle.TangentMarkerRadius, color, 12, *thickness);
}

CCurveEditorTangentView::CCurveEditorTangentView(ICurveEditorView& editorView) :
    CCurveEditorSplineComponentViewBase(editorView)
{
}

bool CCurveEditorTangentView::IsColliding(const ax::pointf& position, float extraThickness /*= 0.0f*/) const noexcept
{
    const auto tangentPosition = GetEditorTangentPosition(false);
    EDITOR_ASSERT(tangentPosition);
    if (!tangentPosition)
        return false;

    const auto& editorView = GetEditorView();

    const auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto distance = (*tangentPosition - position).cwise_product(windowCanvas.GetZoom());
    const auto tangentRadius = editorView.GetEditorStyle().TangentMarkerRadius + extraThickness;

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

        const auto& editorView = GetEditorView();

        const auto tangentRadius = editorView.GetEditorStyle().TangentMarkerRadius;

        const auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();
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

IEditorRenderableUniquePtr CCurveEditorTangentView::CreateBorderRenderable(ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) const
{
    return std::make_unique<CCurveEditorTangentBorderRenderable>(weak_from_this(), borderStyleColor, thicknessStyle);
}

void CCurveEditorTangentView::OnFrame(ImDrawList& drawList)
{
    const auto& style = GetEditorView().GetEditorStyle();

    const auto tangentPosition = GetEditorTangentPosition(true);
    EDITOR_ASSERT(tangentPosition);
    if (!tangentPosition)
        return;

    const auto imGuiTangentPosition = to_imvec(*tangentPosition);

    if (const auto anchorPosition = GetEditorAnchorPointPosition(true))
        drawList.AddLine(to_imvec(*anchorPosition), imGuiTangentPosition, style.Colors[CurveEditorStyleColor_TangentLine], style.TangentLineThickness);

    drawList.AddCircleFilled(imGuiTangentPosition, style.TangentMarkerRadius, style.Colors[CurveEditorStyleColor_TangentMarker]);
}

std::optional<ax::rectf> CCurveEditorTangentView::CalculateBounds(bool screenTranslation) const noexcept
{
    const auto tangentPosition = GetEditorTangentPosition(screenTranslation);
    EDITOR_ASSERT(tangentPosition);
    if (!tangentPosition)
        return std::nullopt;

    const auto& editorView = GetEditorView();
    const auto tangentRadius = editorView.GetEditorStyle().TangentMarkerRadius;

    auto tangentRadiusBounds = ax::pointf{ tangentRadius, tangentRadius };
    return ax::rectf{ tangentPosition->x - tangentRadiusBounds.x, tangentPosition->y - tangentRadiusBounds.x, 2 * tangentRadiusBounds.x, 2 * tangentRadiusBounds.y };
}

std::optional<ax::pointf> CCurveEditorTangentView::GetAnchorPointPosition() const noexcept
{
    if (const auto& controller = GetController())
        return controller->GetAnchorPosition();

    return std::nullopt;
}

std::optional<ax::pointf> CCurveEditorTangentView::GetEditorAnchorPointPosition(bool screenTranslation) const noexcept
{
    return ToEditorPosition(GetAnchorPointPosition(), screenTranslation);
}

std::optional<ax::pointf> CCurveEditorTangentView::GetTangentPosition() const noexcept
{
    if (const auto& controller = GetController())
        return controller->GetPosition();

    return std::nullopt;
}

std::optional<ax::pointf> CCurveEditorTangentView::GetEditorTangentPosition(bool screenTranslation) const noexcept
{
    return ToEditorPosition(GetTangentPosition(), screenTranslation);
}

std::optional<ax::pointf> CCurveEditorTangentView::ToEditorPosition(const std::optional<ax::pointf>& position, bool screenTranslation) const noexcept
{
    if (!position)
        return std::nullopt;

    const auto& editorCanvas = GetEditorView().GetCanvas();
    return editorCanvas.ToEditor(*position, screenTranslation);
}

ICurveEditorTangentViewSharedPtr ICurveEditorTangentView::Create(ICurveEditorView& editorView)
{
    return std::make_shared<CCurveEditorTangentView>(editorView);
}