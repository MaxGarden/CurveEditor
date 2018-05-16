#pragma  once
#if !defined(__CURVE_EDITOR_SELECTION_COMPONENT_H__)

#include "CurveEditorView.h"
#include "SplineController.h"

using CurveEditorViewSelection = const std::set<const ICurveEditorSplineComponentView*>;

class ICurveEditorSelectionViewComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorSelectionViewComponent() override = default;

    virtual void ClearSelection() = 0;

    virtual bool SetSelectionMode(ECurveEditorSplineComponentType selectionMode) noexcept = 0;
    virtual std::optional<ECurveEditorSplineComponentType> GetSelectionMode() const noexcept = 0;

    virtual bool AddToSelection(const CurveEditorViewSelection& splineComponents) = 0;
    virtual bool RemoveFromSelection(const CurveEditorViewSelection& splineComponents) = 0;

    virtual bool CheckIfSelected(const ICurveEditorSplineComponentView& splineComponent) const noexcept = 0;

    virtual void VisitSelection(const VisitorType<ICurveEditorSplineComponentView>& visitor) const = 0;

    static ICurveEditorSelectionViewComponentUniquePtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_SELECTION_COMPONENT_H__