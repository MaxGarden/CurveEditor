#pragma  once
#if !defined(__CURVE_EDITOR_DEBUG_COMPONENT_H__)

#include "CurveEditorViewComponent.h"

class CCurveEditorDebugComponent final : public CCurveEditorViewComponentBase
{
public:
	CCurveEditorDebugComponent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorDebugComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, CCurveEditorController& controller) override final;

private:
	std::string m_SplineName = std::string(128, '\0');
};

#endif //__CURVE_EDITOR_DEBUG_COMPONENT_H__