#pragma  once
#if !defined(__CURVE_EDITOR_BACKGROUND_COMPONENT_H__)

#include "CurveEditorViewComponent.h"

class CCurveEditorBackgroundViewComponent final : public CCurveEditorViewVisibleComponentBase
{
public:
    CCurveEditorBackgroundViewComponent(ICurveEditorView& editorView);
    virtual ~CCurveEditorBackgroundViewComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorController& editorController) override final;
};

#endif //__CURVE_EDITOR_BACKGROUND_COMPONENT_H__