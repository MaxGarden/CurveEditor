#include "pch.h"
#include "CurveEditorSelectionToolBase.h"
#include "CurveEditorView.h"
#include "ImGuiInterop.h"
#include "CurveEditorViewVisibleComponentBase.h"

using namespace ax::ImGuiInterop;

class CCurveEditorSelectionToolBaseViewComponent final : public CCurveEditorViewVisibleComponentBase<>
{
public:
    CCurveEditorSelectionToolBaseViewComponent(ICurveEditorView& editorView, CCurveEditorSelectionToolBase& selectionTool);
    virtual ~CCurveEditorSelectionToolBaseViewComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorController& controller) override final;

private:
    CCurveEditorSelectionToolBase& m_SelectionTool;
};

CCurveEditorSelectionToolBaseViewComponent::CCurveEditorSelectionToolBaseViewComponent(ICurveEditorView& editorView, CCurveEditorSelectionToolBase& selectionTool) :
    CCurveEditorViewVisibleComponentBase(editorView),
    m_SelectionTool(selectionTool)
{
}

void CCurveEditorSelectionToolBaseViewComponent::OnFrame(ImDrawList& drawList, ICurveEditorController&)
{
    const auto selectionRect = m_SelectionTool.GetSelectionRect();
    EDITOR_ASSERT(selectionRect);
    if (!selectionRect)
        return;

    const auto& editorView = GetEditorView();
    const auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto topLeft = to_imvec(windowCanvas.ToScreen(selectionRect->top_left()));
    const auto bottomRight = to_imvec(windowCanvas.ToScreen(selectionRect->bottom_right()));

    const auto& editorStyle = editorView.GetEditorStyle();

    const auto selectionRectColor = editorStyle.Colors[CurveEditorStyleColor_SelectionRect];
    drawList.AddRectFilled(topLeft, bottomRight, selectionRectColor);

    const auto selectionRectBorderColor = editorStyle.Colors[CurveEditorStyleColor_SelectionRectBorder];
    drawList.AddRect(topLeft, bottomRight, selectionRectBorderColor);
}

CCurveEditorSelectionToolBase::CCurveEditorSelectionToolBase(ECurveEditorMouseButton activationMouseButton) :
    m_ActivationMouseButton(activationMouseButton)
{
}

void CCurveEditorSelectionToolBase::OnDragBegin(const CCurveEditorToolMouseButtonEvent& event)
{
    if(!CheckActivationButton(event))
        return;

    if (!(m_SelectionAccepted = AcceptSelection(event)))
        return;

    m_DragStartPosition = event.GetMousePosition();

    EDITOR_ASSERT(!m_SelectionViewHandle);
    if (m_SelectionViewHandle)
        RemoveToolView(*m_SelectionViewHandle);

    auto& editorView = event.GetEditorView();

    auto selectionToolViewComponent = std::make_unique<CCurveEditorSelectionToolBaseViewComponent>(editorView, *this);
    m_SelectionViewHandle = AddToolView(editorView, std::move(selectionToolViewComponent), EComponentOrder::Foreground);

    OnSelectionBegin(editorView);
}

void CCurveEditorSelectionToolBase::OnDragUpdate(const CCurveEditorToolMouseDragEvent& event)
{
    if (!m_SelectionAccepted || !CheckActivationButton(event))
        return;

    const auto& mousePosition = event.GetMousePosition();

    const auto top = std::min(m_DragStartPosition.y, mousePosition.y);
    const auto bottom = std::max(m_DragStartPosition.y, mousePosition.y);

    const auto left = std::min(m_DragStartPosition.x, mousePosition.x);
    const auto right = std::max(m_DragStartPosition.x, mousePosition.x);

    m_SelectionRect = ax::rectf{ left, top, std::max(right - left, 1.0f), std::max(bottom - top, 1.0f) };

    OnSelectionUpdate(event.GetEditorView(), *m_SelectionRect);
}

void CCurveEditorSelectionToolBase::OnDragEnd(const CCurveEditorToolMouseButtonEvent& event)
{
    if (!m_SelectionAccepted || !CheckActivationButton(event))
        return;

    OnSelectionEnd(event.GetEditorView());

    EDITOR_ASSERT(m_SelectionViewHandle);
    if (m_SelectionViewHandle)
    {
        RemoveToolView(*m_SelectionViewHandle);
        m_SelectionViewHandle.reset();
    }

    m_SelectionRect.reset();
}

const std::optional<ax::rectf>& CCurveEditorSelectionToolBase::GetSelectionRect() const noexcept
{
    return m_SelectionRect;
}

bool CCurveEditorSelectionToolBase::AcceptSelection(const CCurveEditorToolMouseButtonEvent&)
{
    //to override
    return true;
}

void CCurveEditorSelectionToolBase::OnSelectionBegin(ICurveEditorView&)
{
    //to override
}

void CCurveEditorSelectionToolBase::OnSelectionUpdate(ICurveEditorView&, const ax::rectf&)
{
    //to override
}

void CCurveEditorSelectionToolBase::OnSelectionEnd(ICurveEditorView&)
{
    //to override
}

bool CCurveEditorSelectionToolBase::CheckActivationButton(const CCurveEditorToolMouseButtonEvent& event) const noexcept
{
    return event.GetMouseButton() == m_ActivationMouseButton;
}