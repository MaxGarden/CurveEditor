#include "pch.h"
#include "RulerComponent.h"
#include "CurveEditorViewVisibleComponentBase.h"
#include "ImGuiInterop.h"

using namespace ImGuiInterop;
using namespace ax::ImGuiInterop;

class CCurveEditorRulerViewComponent final : public CCurveEditorViewVisibleComponentBase<ICurveEditorRulerViewComponent>
{
public:
    CCurveEditorRulerViewComponent(ICurveEditorView& editorView);
    virtual ~CCurveEditorRulerViewComponent() override final = default;

private:
    void DrawVerticalRuler(ImDrawList& drawList, const ImVec2& position, const ImVec2& size, bool leftGraduation = true);
    void DrawHorizontalRuler(ImDrawList& drawList, const ImVec2& position, const ImVec2& size, bool bottomGraduation = true);

protected:
    void OnFrame(ImDrawList& drawList) override final;

private:
    const ax::pointf m_Graduation = { 10.0f, 10.0f };
};

CCurveEditorRulerViewComponent::CCurveEditorRulerViewComponent(ICurveEditorView& editorView) :
    CCurveEditorViewVisibleComponentBase(editorView)
{
}

void CCurveEditorRulerViewComponent::OnFrame(ImDrawList& drawList)
{
    const auto& editorView = GetEditorView();
    const auto& editorStyle = editorView.GetEditorStyle();

    const auto horizontalRulerHeight = editorStyle.RulerSizeFactor * ImGui::GetFontSize();
    const auto verticalRulerWidth = horizontalRulerHeight;

    const auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto& windowSize = windowCanvas.GetWindowScreenSize();

    DrawVerticalRuler(drawList, ImVec2{ 0.0f, horizontalRulerHeight }, ImVec2{ verticalRulerWidth, windowSize.h - 2 * horizontalRulerHeight }, true);
    DrawVerticalRuler(drawList, ImVec2{ windowSize.w - verticalRulerWidth, horizontalRulerHeight }, ImVec2{ verticalRulerWidth, windowSize.h - 2 * horizontalRulerHeight }, false);
    DrawHorizontalRuler(drawList, ImVec2{ 0.0f, windowSize.h - horizontalRulerHeight }, ImVec2{ windowSize.w, horizontalRulerHeight }, true);
    DrawHorizontalRuler(drawList, ImVec2{ 0.0f, 0.0f }, ImVec2{ windowSize.w, horizontalRulerHeight }, false);
}

void CCurveEditorRulerViewComponent::DrawVerticalRuler(ImDrawList& drawList, const ImVec2& position, const ImVec2& size, bool leftGraduation /*= true*/)
{
    const auto graduation = m_Graduation.y;
    EDITOR_ASSERT(graduation != 0.0f);
    if (graduation == 0.0f)
        return;

    const auto& editorView = GetEditorView();
    const auto& editorCanvas = editorView.GetCanvas();

    const auto unitScaler = editorCanvas.GetUnitScaler().y;
    EDITOR_ASSERT(unitScaler > 0);
    if (unitScaler <= 0)
        return;

    const auto& windowCanvas = editorCanvas.GetWindowCanvas();

    const auto windowSize = windowCanvas.GetWindowScreenSize();
    const auto windowPosition = to_imvec(windowCanvas.GetWindowScreenPosition());

    const auto clientOrigin = windowCanvas.GetClientOrigin().y;

    const auto& editorStyle = editorView.GetEditorStyle();

    ImGui::PushClipRect(position + windowPosition, position + size + windowPosition, false);

    drawList.AddRectFilled(position + windowPosition, position + size + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerBackground]);

    const auto zoom = windowCanvas.GetZoom().y;
    EDITOR_ASSERT(zoom != 0.0f);
    if (zoom == 0.0f)
        return;

    const auto absoluteUnit = editorCanvas.CalculateScaledUnit().y;
    EDITOR_ASSERT(absoluteUnit != 0.0f);
    if (absoluteUnit == 0.0f)
        return;

    const auto unit = absoluteUnit / zoom;
    auto index = static_cast<int>((-(clientOrigin) / absoluteUnit));

    for (auto y = -absoluteUnit; y < position.y + size.y + absoluteUnit; y += absoluteUnit)
    {
        const auto currentY = floorf(y + fmodf(clientOrigin, absoluteUnit));
        drawList.AddLine(ImVec2(position.x, currentY) + windowPosition, ImVec2(position.x + size.x, currentY) + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerBigGraduation]);

        const auto currentAbsoluteY = (-1 * (index - 1) * unit) / unitScaler;
        char currentValue[33];
        sprintf(currentValue, "%g", currentAbsoluteY);

        const auto currentGraduation = absoluteUnit / graduation;

        const auto positionXFirst = position.x + (leftGraduation ? 0.0f : size.x);
        const auto positionXSecond = position.x + size.x * (leftGraduation ? 0.25f : 0.75f);

        for (auto i = 1; i < graduation; ++i)
        {
            const auto positionY = floorf(currentY + currentGraduation * i);
            drawList.AddLine(ImVec2(positionXFirst, positionY) + windowPosition, ImVec2(positionXSecond, positionY) + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerSmallGraduation]);
        }

        drawList.AddText(ImVec2(position.x + 3, currentY) + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerText], currentValue);

        ++index;
    }

    drawList.AddRect(position + windowPosition, position + size + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerBorder]);

    ImGui::PopClipRect();
}

void CCurveEditorRulerViewComponent::DrawHorizontalRuler(ImDrawList& drawList, const ImVec2& position, const ImVec2& size, bool bottomGraduation /*= true*/)
{
    const auto graduation = m_Graduation.x;
    EDITOR_ASSERT(graduation != 0.0f);
    if (graduation == 0.0f)
        return;

    const auto& editorView = GetEditorView();
    const auto& editorCanvas = editorView.GetCanvas();

    const auto unitScaler = editorCanvas.GetUnitScaler().x;
    EDITOR_ASSERT(unitScaler > 0);
    if (unitScaler <= 0)
        return;

    const auto& windowCanvas = editorCanvas.GetWindowCanvas();

    const auto windowSize = windowCanvas.GetWindowScreenSize();
    const auto windowPosition = to_imvec(windowCanvas.GetWindowScreenPosition());

    const auto clientOrigin = windowCanvas.GetClientOrigin().x;

    const auto& editorStyle = editorView.GetEditorStyle();

    ImGui::PushClipRect(position + windowPosition, position + size + windowPosition, false);

    drawList.AddRectFilled(position + windowPosition, position + size + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerBackground]);

    const auto zoom = windowCanvas.GetZoom().x;
    EDITOR_ASSERT(zoom != 0.0f);
    if (zoom == 0.0f)
        return;

    const auto absoluteUnit = editorCanvas.CalculateScaledUnit().x;
    EDITOR_ASSERT(absoluteUnit != 0.0f);
    if (absoluteUnit == 0.0f)
        return;

    const auto unit = absoluteUnit / zoom;
    auto index = static_cast<int>((-(clientOrigin) / absoluteUnit));

    for (auto x = -absoluteUnit; x < position.x + size.x + absoluteUnit; x += absoluteUnit)
    {
        const auto currentX = floorf(x + fmodf(clientOrigin, absoluteUnit));
        drawList.AddLine(ImVec2{ currentX, position.y } +windowPosition, ImVec2{ currentX, position.y + size.y } + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerBigGraduation]);

        const auto currentAbsoluteX = ((index - 1) * unit) / unitScaler;
        char currentValue[33];
        sprintf(currentValue, "%g", currentAbsoluteX);

        const auto currentGraduation = absoluteUnit / graduation;

        const auto positionYFirst = position.y + (bottomGraduation ? size.y : 0.0f);
        const auto positionYSecond = position.y + size.y * (bottomGraduation ? 0.75f : 0.25f);

        for (auto i = 1; i < graduation; ++i)
        {
            const auto positionX = floorf(currentX + currentGraduation * i);
            drawList.AddLine(ImVec2(positionX, positionYFirst) + windowPosition, ImVec2(positionX, positionYSecond) + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerSmallGraduation]);
        }

        drawList.AddText(ImVec2(currentX + 3, position.y + size.y - ImGui::GetFontSize()) + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerText], currentValue);

        ++index;
    }

    drawList.AddRect(position + windowPosition, position + size + windowPosition, editorStyle.Colors[CurveEditorStyleColor_RulerBorder]);

    ImGui::PopClipRect();
}

ICurveEditorRulerViewComponentUniquePtr ICurveEditorRulerViewComponent::Create(ICurveEditorView& editorView)
{
    return std::make_unique<CCurveEditorRulerViewComponent>(editorView);
}