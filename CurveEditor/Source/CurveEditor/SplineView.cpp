#include "pch.h"
#include "SplineView.h"
#include "SplineDataModel.h"
#include "SplineController.h"
#include <imgui/imgui.h>
#include <ImGuiInterop.h>

using namespace ax::ImGuiInterop;


CCurveEditorView& CCurveEditorSplineViewBase::GetEditorView() noexcept
{
    return m_EditorView;
}

void CCurveEditorSplineViewBase::OnFrame()
{
    if (const auto controller = GetController())
        OnFrame(*controller);
}

void CCurveEditorSplineViewBase::OnFrame(ICurveEditorSplineController& controller)
{
    //to override
}

CCurveEditorKnotView::CCurveEditorKnotView(CCurveEditorView& editorView, size_t knotIndex) :
    CCurveEditorSplineViewBase(editorView),
    m_KnotIndex(knotIndex)
{
}

void CCurveEditorKnotView::OnFrame(ICurveEditorSplineController& controller)
{
    const auto position = controller.GetKnot(m_KnotIndex);
    EDITOR_ASSERT(position);
    if (!position)
        return;

   // controller
}

CCurveEditorCurveView::CCurveEditorCurveView(CCurveEditorView& editorView, size_t curveIndex) :
    CCurveEditorSplineViewBase(editorView),
    m_CurveIndex(curveIndex)
{
}

void CCurveEditorCurveView::OnFrame(ICurveEditorSplineController& controller)
{
    const auto visitationResult = controller.VisitCurvePoints(m_CurveIndex, [](const auto& point)
    {
        ImGui::Text("%f %f", point.x, point.y);
    });

    EDITOR_ASSERT(visitationResult);
}

CCurveEditorSplineView::CCurveEditorSplineView(CCurveEditorView& editorView) :
    CCurveEditorSplineViewBase(editorView)
{
}

void CCurveEditorSplineView::OnFrame(ICurveEditorSplineController& controller)
{
    EnsureCurveViews(controller);
    VisitCurveViews([](auto& view)
    {
        view.OnFrame();
    });
}

void CCurveEditorSplineView::OnControllerChanged()
{
    m_CurvesViews.clear();
}

void CCurveEditorSplineView::EnsureCurveViews(ICurveEditorSplineController& controller)
{
    const auto curvesCount = controller.GetCurvesCount();

    if (curvesCount == m_CurvesViews.size())
        return;

    const auto previousSize = m_CurvesViews.size();
    m_CurvesViews.resize(curvesCount);

    if (curvesCount <previousSize)
        return;

    std::generate(m_CurvesViews.begin() + previousSize, m_CurvesViews.end(), [curveIndex = previousSize, this]() mutable
    {
        auto curveView = std::make_unique<CCurveEditorCurveView>(curveIndex++);

        auto isValid = true;
        isValid &= curveView->SetController(GetController());

        EDITOR_ASSERT(isValid);

        return isValid ? std::move(curveView) : nullptr;
    });
}

void CCurveEditorSplineView::VisitCurveViews(const std::function<void(CCurveEditorCurveView&)>& visitor) noexcept
{
    if (!visitor)
        return;

    for (const auto& curveView : m_CurvesViews)
    {
        if (curveView)
            visitor(*curveView);
    }
}

CCurveEditorSplineViewBase::CCurveEditorSplineViewBase(CCurveEditorView& editorView) :
    m_EditorView(editorView)
{
}