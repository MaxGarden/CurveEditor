#include "pch.h"
#include "CurveEditorKnotInserterTool.h"
#include "Components/SplinesComponent.h"
#include "CurveViewComponent.h"
#include "CurveEditorViewVisibleComponentBase.h"
#include "ImGuiInterop.h"

using namespace ImGuiInterop;
using namespace ax::ImGuiInterop;

class CCurveEditorKnotInserterToolViewComponent final : public CCurveEditorViewVisibleComponentBase<>
{
public:
    CCurveEditorKnotInserterToolViewComponent(ICurveEditorView& editorView, const CCurveEditorKnotInserterTool& inserterTool);
    virtual ~CCurveEditorKnotInserterToolViewComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList) override final;

private:
    const CCurveEditorKnotInserterTool& m_InserterTool;
};

CCurveEditorKnotInserterToolViewComponent::CCurveEditorKnotInserterToolViewComponent(ICurveEditorView& editorView, const CCurveEditorKnotInserterTool& inserterTool) :
    CCurveEditorViewVisibleComponentBase(editorView),
    m_InserterTool(inserterTool)
{
}

void CCurveEditorKnotInserterToolViewComponent::OnFrame(ImDrawList& drawList)
{
    const auto candidatePosition = m_InserterTool.GetCandidateInsertPosition();
    EDITOR_ASSERT(candidatePosition);
    if (!candidatePosition)
        return;

    const auto& editorView = GetEditorView();
    const auto& editorStyle = editorView.GetEditorStyle();
    const auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto helperSize = 0.5f * editorStyle.KnotSize;
    const auto centerPosition = to_imvec(windowCanvas.ToScreen(*candidatePosition));

    drawList.AddRect(centerPosition - helperSize, centerPosition + helperSize, editorStyle.Colors[CurveEditorStyleColor_Helper], 0.0f, 15, editorStyle.HelperBorderThickness);
}

CCurveEditorKnotInserterTool::CCurveEditorKnotInserterTool(ECurveEditorMouseButton insertButton /*= ECurveEditorMouseButton::Left*/) :
    m_InsertButton(insertButton)
{
}

void CCurveEditorKnotInserterTool::OnAcquired(const CCurveEditorToolEvent& event)
{
    m_SpinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(event.GetEditorView());
    EDITOR_ASSERT(!m_SpinesViewComponent.expired());
}

void CCurveEditorKnotInserterTool::OnReleased(const CCurveEditorToolEvent&)
{
    m_SpinesViewComponent.reset();
    ResetToolView();
}

void CCurveEditorKnotInserterTool::OnClickUp(const CCurveEditorToolMouseButtonEvent& event)
{
    if (event.GetMouseButton() != m_InsertButton)
        return;

    const auto curveView = GetCurveViewAtPosition(event.GetMousePosition());
    if (!curveView)
        return;

    EDITOR_ASSERT(m_CandidateInsertPosition);
    if (!m_CandidateInsertPosition)
        return;

    const auto& editorCanvas = event.GetEditorView().GetCanvas();

    const auto result = curveView->InsertKnot(editorCanvas.FromEditor(*m_CandidateInsertPosition, false).x);
    EDITOR_ASSERT(result);
}

void CCurveEditorKnotInserterTool::OnMouseMove(const CCurveEditorToolMouseEvent& event)
{
    const auto curveView = GetCurveViewAtPosition(event.GetMousePosition());
    if (!curveView)
        return ResetToolView();

    EnsureToolView(event.GetEditorView());

    m_CandidateInsertPosition = curveView->GetClosestPosition(event.GetMousePosition());
    EDITOR_ASSERT(m_CandidateInsertPosition);
}

ICurveEditorCurveView * CCurveEditorKnotInserterTool::GetCurveViewAtPosition(const ax::pointf& position) const noexcept
{
    const auto splinesViewComponent = m_SpinesViewComponent.lock();
    EDITOR_ASSERT(splinesViewComponent);
    if (!splinesViewComponent)
        return nullptr;

    const auto splineViewComponent = splinesViewComponent->GetSplineComponentAt(position);
    if (!splineViewComponent || splineViewComponent->GetType() != ECurveEditorSplineComponentType::Curve)
        return nullptr;

    const auto result = dynamic_cast<ICurveEditorCurveView*>(splineViewComponent);
    EDITOR_ASSERT(result);

    return result;
}

std::optional<ax::pointf> CCurveEditorKnotInserterTool::GetCandidateInsertPosition() const noexcept
{
    return m_CandidateInsertPosition;
}

void CCurveEditorKnotInserterTool::EnsureToolView(ICurveEditorView& editorView)
{
    if (m_ToolViewHandle)
        return;

    m_ToolViewHandle = AddToolView(editorView, std::make_unique<CCurveEditorKnotInserterToolViewComponent>(editorView, *this), EComponentOrder::Helper);
    EDITOR_ASSERT(m_ToolViewHandle);
}

void CCurveEditorKnotInserterTool::ResetToolView()
{
    if (!m_ToolViewHandle)
        return;

    RemoveToolView(*m_ToolViewHandle);
    m_ToolViewHandle.reset();
}