#pragma  once
#if !defined(__CURVE_EDITOR_TOOL_HANDLER_COMPONENT_H__)

#include "CurveEditorToolEvent.h"
#include "CurveEditorViewComponent.h"

class CCurveEditorToolHandlerComponent;

class CMouseButtonHandler
{
public:
    CMouseButtonHandler(ICurveEditorView& editorView, const CCurveEditorToolHandlerComponent& toolHandler, ECurveEditorMouseButton button);
    virtual ~CMouseButtonHandler() = default;

    void OnCapture();
    void Update(ICurveEditorTool& activeTool);
    void OnRelease();

    bool IsDragging() const noexcept;
    ECurveEditorMouseButton GetButton() const noexcept;

private:
    size_t GetButtonIndex() const noexcept;

private:
    ICurveEditorView& m_EditorView;
    const CCurveEditorToolHandlerComponent& m_ToolHandler;
    bool m_IsDragging = false;
    bool m_WasDragging = false;
    ECurveEditorMouseButton m_Button;
    ImVec2 m_ClickPositionBuffer;
    ax::pointf m_DragDelta;
};

class CCurveEditorToolHandlerComponent final : public CEditorViewBase<IEditorView, ICurveEditorController>
{
public:
    CCurveEditorToolHandlerComponent(ICurveEditorView& editorView);
    virtual ~CCurveEditorToolHandlerComponent() override final = default;

    virtual void OnFrame() override final;

    void VisitButtonHandlers(const ConstVisitorType<CMouseButtonHandler>& visitor) const noexcept;

protected:
    void OnFrame(ICurveEditorController& editorController);

private:
    void CaptureMouseState();
    void UpdateMouseState(ICurveEditorController& editorController);
    void UpdateWheelState(ICurveEditorTool& activeTool);
    void ReleaseMouseState();

private:
    ICurveEditorView& m_EditorView;
    ImVec2 m_MousePositionBuffer;
    ImVec2 m_MouseClicksPositionBuffers[5];
    std::vector<CMouseButtonHandler> m_ButtonHandlers;
};

#endif //__CURVE_EDITOR_TOOL_HANDLER_COMPONENT_H__