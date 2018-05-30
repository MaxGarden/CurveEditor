#include "pch.h"
#include "KnotViewComponent.h"
#include "CurveEditorView.h"
#include "KnotController.h"
#include "EditorRenderableBase.h"
#include "CurveEditorViewVisibleComponentBase.h"
#include "SplineComponentViewBase.h"
#include <ImGuiInterop.h>

using namespace ax::ImGuiInterop;

DECLARE_POINTERS(CCurveEditorKnotView);

class CCurveEditorKnotBorderRenderable final : public CCurveEditorSplineComponentBorderRenderableBase<CCurveEditorKnotView>
{
public:
    CCurveEditorKnotBorderRenderable(CCurveEditorKnotViewConstWeakPtr&& knotView, ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle);
    virtual ~CCurveEditorKnotBorderRenderable() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList) override final;
};

class CCurveEditorKnotView final : public CCurveEditorSplineComponentViewBase<ICurveEditorKnotView, ICurveEditorKnotController, ECurveEditorSplineComponentType::Knot>, public std::enable_shared_from_this<CCurveEditorKnotView>
{
friend CCurveEditorKnotBorderRenderable;
public:
    CCurveEditorKnotView(ICurveEditorView& editorView);
    virtual ~CCurveEditorKnotView() override final = default;

    virtual bool IsColliding(const ax::pointf& position, float extraThickness = 0.0f) const noexcept override final;
    virtual bool IsColliding(const ax::rectf& rect, bool allowIntersect = true) const noexcept override final;

    virtual bool CanBeRemoved() const noexcept override final;
    virtual bool Remove() override final;

protected:
    virtual void OnFrame(ImDrawList& drawList) override final;

private:
    virtual IEditorRenderableUniquePtr CreateBorderRenderable(ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) const override final;

    std::optional<ax::pointf> GetEditorPosition(bool screenTranslation) const noexcept;

    std::optional<ax::rectf> CalculateBounds(bool screenTranslation) const noexcept;
};

CCurveEditorKnotBorderRenderable::CCurveEditorKnotBorderRenderable(CCurveEditorKnotViewConstWeakPtr&& knotView, ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) :
    CCurveEditorSplineComponentBorderRenderableBase(std::move(knotView), borderStyleColor, thicknessStyle)
{
}

void CCurveEditorKnotBorderRenderable::OnFrame(ImDrawList& drawList)
{
    const auto knotView = GetSplineComponentView().lock();
    if (!knotView)
        return;

    const auto bounds = knotView->CalculateBounds(true);
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return;

    const auto& editorStyle = knotView->GetEditorView().GetEditorStyle();

    const auto thickness = editorStyle.GetEditorStyleFloat(GetThichnessStyle());
    if (!thickness || thickness <= 0.0f)
        return;

    const auto& color = editorStyle.Colors[GetBorderStyleColor()];

    drawList.AddRect(to_imvec(bounds->top_left()), to_imvec(bounds->bottom_right()), color, 0.0f, 15, *thickness);
}

CCurveEditorKnotView::CCurveEditorKnotView(ICurveEditorView& editorView) :
    CCurveEditorSplineComponentViewBase(editorView)
{
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

bool CCurveEditorKnotView::CanBeRemoved() const noexcept
{
    if (const auto& controller = GetController())
        return controller->CanBeRemoved();

    EDITOR_ASSERT(false);
    return false;
}

bool CCurveEditorKnotView::Remove()
{
    if (const auto& controller = GetController())
        return controller->Remove();

    EDITOR_ASSERT(false);
    return false;
}

IEditorRenderableUniquePtr CCurveEditorKnotView::CreateBorderRenderable(ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) const
{
    return std::make_unique<CCurveEditorKnotBorderRenderable>(weak_from_this(), borderStyleColor, thicknessStyle);
}

std::optional<ax::pointf> CCurveEditorKnotView::GetEditorPosition(bool screenTranslation) const noexcept
{
    const auto position = GetPosition();
    if (!position)
        return std::nullopt;

    const auto& editorCanvas = GetEditorView().GetCanvas();
    return editorCanvas.ToEditor(*position, screenTranslation);
}

std::optional<ax::rectf> CCurveEditorKnotView::CalculateBounds(bool screenTranslation) const noexcept
{
    const auto editorPosition = GetEditorPosition(screenTranslation);
    if (!editorPosition)
        return std::nullopt;

    const auto& editorView = GetEditorView();

    auto halfKnotSize = to_pointf(editorView.GetEditorStyle().KnotSize) * 0.5;

    if (!screenTranslation)
    {
        const auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();
        halfKnotSize = halfKnotSize.cwise_product(windowCanvas.GetInvertZoom());
    }

    return ax::rectf{ *editorPosition - halfKnotSize, *editorPosition + halfKnotSize };
}

void CCurveEditorKnotView::OnFrame(ImDrawList& drawList)
{
    const auto bounds = CalculateBounds(true);
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return;

    const auto& editorStyle = GetEditorView().GetEditorStyle();
    const auto& fillColor = editorStyle.Colors[CurveEditorStyleColor_Knot];

    drawList.AddRectFilled(to_imvec(bounds->top_left()), to_imvec(bounds->bottom_right()), fillColor);
}

ICurveEditorKnotViewSharedPtr ICurveEditorKnotView::Create(ICurveEditorView& editorView)
{
    return std::make_shared<CCurveEditorKnotView>(editorView);
}