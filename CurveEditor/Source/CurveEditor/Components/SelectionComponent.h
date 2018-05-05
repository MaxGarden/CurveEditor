#pragma  once
#if !defined(__CURVE_EDITOR_SELECTION_COMPONENT_H__)

#include "CurveEditorView.h"

class ICurveEditorSelectionViewComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorSelectionViewComponent() override = default;

    virtual void ClearSelection() = 0;

    virtual bool AddToSelection(const ICurveEditorSplineComponentView& splineComponent) = 0;
    virtual bool AddToSelection(const std::set<const ICurveEditorSplineComponentView*>& splineComponents) = 0;

    virtual bool RemoveFromSelection(const ICurveEditorSplineComponentView& splineComponent) = 0;
    virtual bool RemoveFromSelection(const std::set<const ICurveEditorSplineComponentView*>& splineComponents) = 0;

    virtual bool CheckIfSelected(const ICurveEditorSplineComponentView& splineComponent) const noexcept = 0;

    static ICurveEditorSelectionViewComponentUniquePtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_SELECTION_COMPONENT_H__