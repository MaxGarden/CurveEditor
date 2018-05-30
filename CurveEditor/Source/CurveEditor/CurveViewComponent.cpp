#include "pch.h"
#include "CurveViewComponent.h"
#include "CurveController.h"
#include "CurveEditorView.h"
#include "EditorRenderableBase.h"
#include "SplineComponentViewBase.h"
#include <ImGuiInterop.h>

using namespace ax::ImGuiInterop;

DECLARE_POINTERS(CCurveEditorCurveView);

class CCurveEditorCurveBorderRenderable final : public CCurveEditorSplineComponentBorderRenderableBase<CCurveEditorCurveView>
{
public:
    CCurveEditorCurveBorderRenderable(CCurveEditorCurveViewConstWeakPtr&& curveView, ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle);
    virtual ~CCurveEditorCurveBorderRenderable() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList) override final;
};

class CCurveEditorCurveView final : public CCurveEditorSplineComponentViewBase<ICurveEditorCurveView, ICurveEditorCurveController,ECurveEditorSplineComponentType::Curve>, public std::enable_shared_from_this<CCurveEditorCurveView>
{
friend CCurveEditorCurveBorderRenderable;
public:
    CCurveEditorCurveView(ICurveEditorView& editorView);
    virtual ~CCurveEditorCurveView() override final = default;

    virtual bool IsColliding(const ax::pointf& point, float extraThickness = 0.0f) const noexcept override final;
    virtual bool IsColliding(const ax::rectf& rect, bool allowIntersect = true) const noexcept override final;

    virtual bool InsertKnot(float position) override final;
    virtual std::optional<ax::pointf> GetClosestPosition(const ax::pointf& position) const noexcept override final;

    virtual IEditorRenderableUniquePtr CreateBorderRenderable(ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) const override final;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorCurveController& splineController) override final;

private:
    std::optional<float> EvaluateClosestT(const ax::cubic_bezier_t& curve, float x, float precision = 0.00001f) const noexcept;

private:
    std::optional<ax::rectf> CalculateBounds(bool screenTranslation) const noexcept;

    std::optional<ax::cubic_bezier_t> GetControlPointsPositions() const noexcept;
    std::optional<ax::cubic_bezier_t> GetEditorControlPointsPositions(bool screenTranslation) const noexcept;
};

CCurveEditorCurveBorderRenderable::CCurveEditorCurveBorderRenderable(CCurveEditorCurveViewConstWeakPtr&& curveView, ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) :
    CCurveEditorSplineComponentBorderRenderableBase(std::move(curveView), borderStyleColor, thicknessStyle)
{
}

void CCurveEditorCurveBorderRenderable::OnFrame(ImDrawList& drawList)
{
    const auto curveView = GetSplineComponentView().lock();
    if (!curveView)
        return;

    const auto controlPoints = curveView->GetEditorControlPointsPositions(true);
    EDITOR_ASSERT(controlPoints);
    if (!controlPoints)
        return;

    const auto& editorStyle = curveView->GetEditorView().GetEditorStyle();

    const auto thickness = editorStyle.GetEditorStyleFloat(GetThichnessStyle());
    if (!thickness || thickness <= 0.0f)
        return;

    const auto& color = editorStyle.Colors[GetBorderStyleColor()];

    const auto getControlPoint = [&controlPoints](size_t index)
    {
        return to_imvec(*(&controlPoints->p0 + index));
    };

    drawList.AddBezierCurve(getControlPoint(0), getControlPoint(1), getControlPoint(2), getControlPoint(3), color, *thickness);
}

CCurveEditorCurveView::CCurveEditorCurveView(ICurveEditorView& editorView) :
    CCurveEditorSplineComponentViewBase(editorView)
{
}

bool CCurveEditorCurveView::IsColliding(const ax::pointf& position, float extraThickness /*= 0.0f*/) const noexcept
{
    auto bounds = CalculateBounds(false);
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return false;

    if (extraThickness > 0.0f)
        bounds->expand(extraThickness);

    if (!bounds->contains(position))
        return false;

    const auto& editorView = GetEditorView();

    const auto controlPoints = GetEditorControlPointsPositions(false);
    const auto distance = ax::cubic_bezier_project_point(position, controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, 10000).distance;

    const auto splineThickness = editorView.GetEditorStyle().SplineThickness;

    const auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();
    const auto splineThickneessBounds = ax::pointf{ splineThickness, splineThickness }.cwise_product(windowCanvas.GetInvertZoom()) + ax::pointf{ extraThickness, extraThickness };

    return distance <= splineThickneessBounds.x && distance <= splineThickneessBounds.y;
}

bool CCurveEditorCurveView::IsColliding(const ax::rectf& rect, bool allowIntersect /*= true*/) const noexcept
{
    const auto bounds = CalculateBounds(false);
    EDITOR_ASSERT(bounds);
    if (!bounds)
        return false;

    if (rect.contains(*bounds))
        return true;

    if (!allowIntersect || !rect.intersects(*bounds))
        return false;

    const auto controlPoints = GetEditorControlPointsPositions(false);

    const auto topLeft = rect.top_left();
    const auto topRight = rect.top_right();
    const auto bottomRight = rect.bottom_right();
    const auto bottomLeft = rect.bottom_left();

    std::array<ax::pointf, 3> points;

    if (ax::cubic_bezier_line_intersect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, topLeft, topRight, points.data()) > 0)
        return true;
    if (ax::cubic_bezier_line_intersect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, topRight, bottomRight, points.data()) > 0)
        return true;
    if (ax::cubic_bezier_line_intersect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, bottomRight, bottomLeft, points.data()) > 0)
        return true;
    if (ax::cubic_bezier_line_intersect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, bottomLeft, topLeft, points.data()) > 0)
        return true;

    return false;
}

IEditorRenderableUniquePtr CCurveEditorCurveView::CreateBorderRenderable(ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle) const
{
    return std::make_unique<CCurveEditorCurveBorderRenderable>(weak_from_this(), borderStyleColor, thicknessStyle);
}

std::optional<ax::rectf> CCurveEditorCurveView::CalculateBounds(bool screenTranslation) const noexcept
{
    const auto controlPoints = GetEditorControlPointsPositions(screenTranslation);
    if (!controlPoints)
        return std::nullopt;

    auto result = ax::cubic_bezier_bounding_rect(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3);

    if (result.w == 0.0f)
    {
        result.x -= 0.5f;
        result.w = 1.0f;
    }

    if (result.h == 0.0f)
    {
        result.y -= 0.5f;
        result.h = 1.0f;
    }

    result.expand(GetEditorView().GetEditorStyle().SplineThickness);

    return result;
}

std::optional<ax::cubic_bezier_t> CCurveEditorCurveView::GetControlPointsPositions() const noexcept
{
    const auto& controller = GetController();
    if (!controller)
        return std::nullopt;

    std::array<ax::pointf, 4u> controlPoints;

    const auto visitationResult = controller->VisitCurveControlPoints([iterator = controlPoints.begin(), endIterator = controlPoints.end()](const auto& point) mutable
    {
        EDITOR_ASSERT(iterator != endIterator);
        if (iterator != endIterator)
            *(iterator++) = point;
    });

    EDITOR_ASSERT(visitationResult);
    if (!visitationResult)
        return std::nullopt;

    return ax::cubic_bezier_t{ controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3] };
}

std::optional<ax::cubic_bezier_t> CCurveEditorCurveView::GetEditorControlPointsPositions(bool screenTranslation) const noexcept
{
    auto controlPoints = GetControlPointsPositions();
    if (!controlPoints)
        return std::nullopt;

    const auto& editorCanvas = GetEditorView().GetCanvas();

    const auto translatePoint = [&editorCanvas, &screenTranslation](auto& point)
    {
        point = editorCanvas.ToEditor(point, screenTranslation);
    };

    const auto first = &controlPoints->p0;

    for (auto i = 0u; i < 4; ++i)
        translatePoint(*(first + i));;

    return controlPoints;
}

void CCurveEditorCurveView::OnFrame(ImDrawList& drawList, ICurveEditorCurveController& splineController)
{
    const auto controlPoints = GetEditorControlPointsPositions(true);
    EDITOR_ASSERT(controlPoints);
    if (!controlPoints)
        return;

    const auto& editorStyle = GetEditorView().GetEditorStyle();
    const auto splineThickness = editorStyle.SplineThickness;

    const auto curveColor = splineController.GetColor();

    const auto getControlPoint = [&controlPoints](size_t index)
    {
        return to_imvec(*(&controlPoints->p0 + index));
    };

    drawList.AddBezierCurve(getControlPoint(0), getControlPoint(1), getControlPoint(2), getControlPoint(3), curveColor, splineThickness);
}

bool CCurveEditorCurveView::InsertKnot(float position)
{
    const auto controlPoints = GetControlPointsPositions();
    EDITOR_ASSERT(controlPoints);
    if (!controlPoints)
        return false;

    const auto isPositionValid = position > controlPoints->p0.x && position < controlPoints->p3.x;
    EDITOR_ASSERT(isPositionValid);
    if (!isPositionValid)
        return false;

    const auto tPosition = EvaluateClosestT(*controlPoints, position);
    EDITOR_ASSERT(tPosition);
    if (!tPosition)
        return false;

    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return false;

    return controller->InsertKnot(*tPosition);
}

std::optional<ax::pointf> CCurveEditorCurveView::GetClosestPosition(const ax::pointf& position) const noexcept
{
    auto controlPoints = GetEditorControlPointsPositions(false);
    EDITOR_ASSERT(controlPoints);
    if (!controlPoints)
        return std::nullopt;

    const auto projectionResult = ax::cubic_bezier_project_point(position, controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3);
    return ax::cubic_bezier(controlPoints->p0, controlPoints->p1, controlPoints->p2, controlPoints->p3, projectionResult.position);
}

std::optional<float> CCurveEditorCurveView::EvaluateClosestT(const ax::cubic_bezier_t& curve, float x, float precision /*= 0.00001f*/) const noexcept
{
    auto step = 0.5f;
    auto accumulator = 0.0f;

    while (true)
    {
        auto point = cubic_bezier(curve.p0, curve.p1, curve.p2, curve.p3, 0.5f - accumulator);

        if (step == 0.0f)
            break;

        step *= 0.5f;

        if (point.x < x)
            accumulator -= step;
        else if (point.x > x)
            accumulator += step;

        if (abs(point.x - x) < precision)
            return 0.5f - accumulator;
    }

    return std::nullopt;
}

ICurveEditorCurveViewSharedPtr ICurveEditorCurveView::Create(ICurveEditorView& editorView)
{
    return std::make_shared<CCurveEditorCurveView>(editorView);
}