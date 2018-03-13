#pragma  once
#if !defined(__CURVE_EDITOR_BACKGROUND_COMPONENT_H__)

#include "CurveEditorViewComponent.h"

class CCurveEditorBackgroundViewComponent final : public CCurveEditorViewComponentBase
{
public:
    CCurveEditorBackgroundViewComponent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorBackgroundViewComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, CCurveEditorController& controller) override final;
};

#endif //__CURVE_EDITOR_BACKGROUND_COMPONENT_H__