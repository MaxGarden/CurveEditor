#include "pch.h"
#include "BackgroundComponent.h"
#include "CurveEditorViewVisibleComponentBase.h"
#include "CurveEditorController.h"
#include <ImGuiInterop.h>

using namespace ImGuiInterop;
using namespace ax::ImGuiInterop;

class CCurveEditorBackgroundViewComponent final : public CCurveEditorViewVisibleComponentBase<ICurveEditorBackgroundViewComponent>
{
public:
    CCurveEditorBackgroundViewComponent(ICurveEditorView& editorView);
    virtual ~CCurveEditorBackgroundViewComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList) override final;
};

CCurveEditorBackgroundViewComponent::CCurveEditorBackgroundViewComponent(ICurveEditorView& editorView) :
    CCurveEditorViewVisibleComponentBase(editorView)
{
}

void CCurveEditorBackgroundViewComponent::OnFrame(ImDrawList& drawList)
{
    const auto& editorView = GetEditorView();
    const auto& style = editorView.GetEditorStyle();
    const auto& editorCanvas = editorView.GetCanvas();
    const auto& windowCanvas = editorCanvas.GetWindowCanvas();

    const auto& windowPosition = to_imvec(windowCanvas.GetWindowScreenPosition());
    const auto& windowSize = to_imvec(windowCanvas.GetWindowScreenSize());

    const auto& backgroundColor = style.Colors[CurveEditorStyleColor_Background];

    drawList.AddRectFilled(windowPosition, windowPosition + windowSize, backgroundColor);
}

ICurveEditorBackgroundViewComponentUniquePtr ICurveEditorBackgroundViewComponent::Create(ICurveEditorView& editorView)
{
    return std::make_unique<CCurveEditorBackgroundViewComponent>(editorView);
}