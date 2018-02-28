#pragma  once
#if !defined(__GRID_COMPONENT_H__)

#include "Editor.h"

class CCurveEditorGridViewComponent final : public CCurveEditorViewComponent
{
public:
    CCurveEditorGridViewComponent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorGridViewComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList) override final;
};

#endif //__GRID_COMPONENT_H__