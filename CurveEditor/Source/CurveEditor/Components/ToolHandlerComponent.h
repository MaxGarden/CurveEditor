#pragma  once
#if !defined(__CURVE_EDITOR_TOOL_HANDLER_COMPONENT_H__)

#include "CurveEditorToolEvent.h"
#include "CurveEditorViewComponent.h"

class CCurveEditorToolHandlerComponent;

class CMouseButtonHandler
{
public:
    CMouseButtonHandler(const CCurveEditorView& editorView, const CCurveEditorToolHandlerComponent& toolHandler, ECurveEditorMouseButton button);
    virtual ~CMouseButtonHandler() = default;

    void OnCapture();
    void Update(ICurveEditorTool& activeTool);
    void OnRelease();

    bool IsDragging() const noexcept;
    ECurveEditorMouseButton GetButton() const noexcept;

private:
    size_t GetButtonIndex() const noexcept;

private:
    const CCurveEditorView& m_EditorView;
    const CCurveEditorToolHandlerComponent& m_ToolHandler;
    bool m_IsDragging = false;
    ECurveEditorMouseButton m_Button;
    ImVec2 m_ClickPositionBuffer;
};

class CCurveEditorToolHandlerComponent final : public CCurveEditorViewComponentBase
{
public:
    CCurveEditorToolHandlerComponent(const CCurveEditorView& editorView);
    virtual ~CCurveEditorToolHandlerComponent() override final = default;

    void VisitButtonHandlers(const ConstVisitorType<CMouseButtonHandler>& visitor) const noexcept;

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