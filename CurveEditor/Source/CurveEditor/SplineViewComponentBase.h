#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_COMPONENT_BASE_H__)

#include "Style.h"
#include "EditorViewBase.h"
#include "SplineViewComponent.h"
#include "SplineController.h"

template<typename SuperClass>
class CCurveEditorSplineViewComponentBase : public CEditorViewBase<SuperClass, ICurveEditorSplineController>
{
public:
    CCurveEditorSplineViewComponentBase(ICurveEditorView& editorView);
    virtual ~CCurveEditorSplineViewComponentBase() override = default;

    virtual void OnFrame() override;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller);

    ICurveEditorView& GetEditorView() const noexcept;

    using Super = CCurveEditorSplineViewComponentBase<SuperClass>;

private:
    ICurveEditorView& m_EditorView;
};

template<typename SplineComponentType>
class CCurveEditorSplineComponentBorderRenderableBase : public CEditorRenderableBase
{
    using SplineComponentConstWeakPtr = std::weak_ptr<const SplineComponentType>;

public:
    CCurveEditorSplineComponentBorderRenderableBase(SplineComponentConstWeakPtr && splineComponentView, ECurveEditorStyleColor borderStyleColor, ECurveEditorStyleFloat thicknessStyle);
    virtual ~CCurveEditorSplineComponentBorderRenderableBase() override = default;

protected:
    const SplineComponentConstWeakPtr& GetSplineComponentView() const noexcept;
    ECurveEditorStyleColor GetBorderStyleColor() const noexcept;
    ECurveEditorStyleFloat GetThichnessStyle() const noexcept;

    using Super = CCurveEditorSplineComponentBorderRenderableBase<SplineComponentType>;

private:
    SplineComponentConstWeakPtr m_SplineComponentView;
    const ECurveEditorStyleColor m_BorderStyleColor;
    const ECurveEditorStyleFloat m_ThicknessStyle;
};

#include "SplineViewComponentBase.inl"

#endif //__CURVE_EDITOR_SPLINE_VIEW_COMPONENT_BASE_H__