#pragma  once
#if !defined(__CURVE_EDITOR_TANGENT_VIEW_COMPONENT_H__)

#include "SplineViewComponentBase.h"

class CCurveEditorTangentView final : public CCurveEditorSplineViewComponentBase
{
public:
    CCurveEditorTangentView(ICurveEditorView& editorView, size_t tangentPointIndex);
    virtual ~CCurveEditorTangentView() override final = default;

    virtual bool IsColliding(const ax::pointf& position, float extraThickness = 0.0f) const noexcept override final;
    virtual bool IsColliding(const ax::rectf& rect, bool allowIntersect = true) const noexcept override final;

    virtual ECurveEditorSplineComponentType GetType() const noexcept override final;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller) override final;

private:
    std::optional<ax::pointf> ToEditorPosition(const std::optional<ax::pointf>& position, bool screenTranslation) const noexcept;
    std::optional<ax::rectf> CalculateBounds(bool screenTranslation) const noexcept;

    std::optional<ax::pointf> GetAnchorPointPosition() const noexcept;
    std::optional<ax::pointf> GetEditorAnchorPointPosition(bool screenTranslation) const noexcept;

    std::optional<ax::pointf> GetTangentPosition() const noexcept;
    std::optional<ax::pointf> GetEditorTangentPosition(bool screenTranslation) const noexcept;

private:
    size_t m_AnchorKnotIndex = 0;
    size_t m_TangentIndex = 0;
};

#endif //__CURVE_EDITOR_TANGENT_VIEW_COMPONENT_H__