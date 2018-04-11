#pragma  once
#if !defined(__CURVE_EDITOR_DEBUG_COMPONENT_H__)

#include "CurveEditorViewComponent.h"

class CCurveEditorDebugComponent final : public CCurveEditorViewComponentBase
{
public:
    CCurveEditorDebugComponent(const ICurveEditorView& editorView, IEditorContext& editorContext);
    virtual ~CCurveEditorDebugComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorController& editorController) override final;

private:
    IEditorContext& m_EditorContext;
    std::string m_SplineName = std::string(128, '\0');
    ImColor m_SplineColor = ImColor(0.0f, 1.0f, 0.0f);

    std::stack<ICurveEditorSplineDataModelWeakPtr> m_CreatedSplinesDataModels;
};

#endif //__CURVE_EDITOR_DEBUG_COMPONENT_H__