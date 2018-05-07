#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_COMPONENT_BASE_H__)

#include "Style.h"
#include "EditorViewBase.h"
#include "SplineDataModel.h"
#include "SplineController.h"
#include "SplineComponentView.h"

template<typename SuperClass, typename ControllerType, ECurveEditorSplineComponentType SplineComponentType>
class CCurveEditorSplineComponentViewBase : public CEditorViewBase<SuperClass, ControllerType>
{
public:
    CCurveEditorSplineComponentViewBase(ICurveEditorView& editorView);
    virtual ~CCurveEditorSplineComponentViewBase() override = default;

    virtual void OnFrame() override;

    virtual ECurveEditorSplineComponentType GetType() const noexcept override final;

protected:
    virtual void OnFrame(ImDrawList& drawList, ControllerType& controller);

    ICurveEditorView& GetEditorView() const noexcept;

    using Super = CCurveEditorSplineComponentViewBase<SuperClass, ControllerType, SplineComponentType>;

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

#include "SplineComponentViewBase.inl"

#endif //__CURVE_EDITOR_SPLINE_COMPONENT_VIEW_BASE_H__