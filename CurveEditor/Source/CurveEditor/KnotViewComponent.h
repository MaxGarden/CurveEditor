#pragma  once
#if !defined(__CURVE_EDITOR_KNOT_VIEW_COMPONENT_H__)

#include "SplineViewComponentBase.h"

class CCurveEditorKnotView final : public CCurveEditorSplineViewComponentBase
{
public:
    CCurveEditorKnotView(ICurveEditorView& editorView, size_t knotIndex);
    virtual ~CCurveEditorKnotView() override final = default;

    virtual bool IsColliding(const ax::pointf& position, float extraThickness = 0.0f) const noexcept override final;
    virtual bool IsColliding(const ax::rectf& rect, bool allowIntersect = true) const noexcept override final;

    virtual ECurveEditorSplineComponentType GetType() const noexcept override final;

    std::optional<ax::pointf> GetPosition() const noexcept;
    std::optional<ax::pointf> GetEditorPosition(bool screenTranslation) const noexcept;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller) override final;

private:
    std::optional<ax::rectf> CalculateBounds(bool screenTranslation) const noexcept;

private:
    size_t m_KnotIndex = 0;
};

#endif //__CURVE_EDITOR_KNOT_VIEW_COMPONENT_H__