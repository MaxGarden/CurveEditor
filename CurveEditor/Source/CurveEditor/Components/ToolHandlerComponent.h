#pragma  once
#if !defined(__CURVE_EDITOR_TOOL_HANDLER_COMPONENT_H__)

#include "CurveEditorViewComponent.h"

class CCurveEditorToolHandlerComponent;

using MouseButtonIndex = size_t;

class CMouseButtonHandler
{
public:
    CMouseButtonHandler(CCurveEditorToolHandlerComponent& toolHandlerComponent, MouseButtonIndex buttonIndex);
    virtual ~CMouseButtonHandler() = default;

    void OnCapture(const CCurveEditorView& editorView);
    void Update(ICurveEditorTool& activeTool);
    void OnRelease();

private:
    CCurveEditorToolHandlerComponent& m_ToolHandlerComponent;
    bool m_IsDragging = false;
    MouseButtonIndex m_ButtonIndex = {};
    ImVec2 m_ClickPositionBuffer;
};

class CCurveEditorToolHandlerComponent final : public CCurveEditorViewComponentBase
{
public:
    CCurveEditorToolHandlerComponent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorToolHandlerComponent() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, CCurveEditorController& editorController) override final;

private:
    void CaptureMouseState();
    void UpdateMouseState(CCurveEditorController& editorController);
    void ReleaseMouseState();

private:
    ImVec2 m_MousePositionBuffer;
    ImVec2 m_MouseClicksPositionBuffers[5];
    std::vector<CMouseButtonHandler> m_ButtonHandlers;
};

#endif //__CURVE_EDITOR_TOOL_HANDLER_COMPONENT_H__