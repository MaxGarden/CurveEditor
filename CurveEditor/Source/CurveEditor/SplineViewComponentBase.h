#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_COMPONENT_BASE_H__)

#include "Style.h"
#include "EditorViewBase.h"
#include "SplineViewComponent.h"
#include "SplineController.h"

class CCurveEditorSplineViewComponentBase : public CEditorViewBase<ICurveEditorSplineViewComponent, ICurveEditorSplineController>
{
public:
    CCurveEditorSplineViewComponentBase(ICurveEditorView& editorView);
    virtual ~CCurveEditorSplineViewComponentBase() override = default;

    virtual void OnFrame() override;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller);

    ICurveEditorView& GetEditorView() noexcept;
    const ICurveEditorView& GetEditorView() const noexcept;
    const SCurveEditorStyle& GetEditorStyle() const noexcept;

private:
    ICurveEditorView& m_EditorView;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_COMPONENT_BASE_H__