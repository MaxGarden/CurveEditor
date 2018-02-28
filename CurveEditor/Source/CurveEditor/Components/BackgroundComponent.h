#pragma  once
#if !defined(__BACKGROUND_COMPONENT_H__)

#include "Editor.h"

class CCurveEditorBackgroundViewComponent final : public CCurveEditorViewComponent
{
public:
    CCurveEditorBackgroundViewComponent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorBackgroundViewComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList) override final;
};

#endif //__BACKGROUND_COMPONENT_H__