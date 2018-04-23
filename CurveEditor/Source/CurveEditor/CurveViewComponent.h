#pragma  once
#if !defined(__CURVE_EDITOR_CURVE_VIEW_COMPONENT_H__)

#include "SplineViewComponentBase.h"

class CCurveEditorCurveView final : public CCurveEditorSplineViewComponentBase
{
public:
    CCurveEditorCurveView(ICurveEditorView& editorView, size_t curveIndex);
    virtual ~CCurveEditorCurveView() override final = default;

    virtual bool IsColliding(const ax::pointf& point, float extraThickness = 0.0f) const noexcept override final;
    virtual bool IsColliding(const ax::rectf& rect, bool allowIntersect = true) const noexcept override final;

    virtual ECurveEditorSplineComponentType GetType() const noexcept override final;

    std::optional<ax::cubic_bezier_t> GetControlPointsPositions() const noexcept;
    std::optional<ax::cubic_bezier_t> GetEditorControlPointsPositions() const noexcept;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller) override final;

private:
    std::optional<ax::rectf> CalculateBounds() const noexcept;

private:
    size_t m_CurveIndex = 0;
};

#endif //__CURVE_EDITOR_CURVE_VIEW_COMPONENT_H__