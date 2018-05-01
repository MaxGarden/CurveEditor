#pragma  once
#if !defined(__CURVE_EDITOR_SPLINES_COMPONENT_H__)

#include "CurveEditorView.h"
#include "SplineViewComponent.h"

class ICurveEditorSplinesViewComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorSplinesViewComponent() override = default;

    virtual ICurveEditorSplineViewComponent* GetSplineComponentAt(const ax::pointf& position, std::optional<ECurveEditorSplineComponentType> componentType = std::nullopt, float extraThickness = 0.0f) const noexcept = 0;
    virtual void VisitSplineComponentsInRect(const VisitorType<ICurveEditorSplineViewComponent>& visitor, const ax::rectf& rect, std::optional<ECurveEditorSplineComponentType> componentType = std::nullopt, bool allowIntersect = true) const noexcept = 0;

    static ICurveEditorSplinesViewComponentUniquePtr Create(ICurveEditorView& editorView, ICurveEditorSplineViewFactory& splineViewFactory);
};

#endif //__CURVE_EDITOR_SPLINES_COMPONENT_H__