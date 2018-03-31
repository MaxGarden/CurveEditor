#pragma  once
#if !defined(__CURVE_EDITOR_DEBUG_COMPONENT_H__)

#include "CurveEditorViewComponent.h"

class CCurveEditorDebugComponent final : public CCurveEditorViewComponentBase
{
public:
    CCurveEditorDebugComponent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorDebugComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, CCurveEditorController& editorController) override final;

private:
    std::string m_SplineName = std::string(128, '\0');
    ImColor m_SplineColor = ImColor(1.0f, 0.0f, 0.0f);
};

#endif //__CURVE_EDITOR_DEBUG_COMPONENT_H__