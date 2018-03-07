#pragma  once
#if !defined(__CURVE_EDITOR_GRID_COMPONENT_H__)

#include "CurveEditorViewComponent.h"

class CCurveEditorGridViewComponent final : public CCurveEditorViewComponentBase
{
public:
    CCurveEditorGridViewComponent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorGridViewComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, const CCurveEditorDataModel& dataModel, CCurveEditorController& controller) override final;
};

#endif //__CURVE_EDITOR_GRID_COMPONENT_H__