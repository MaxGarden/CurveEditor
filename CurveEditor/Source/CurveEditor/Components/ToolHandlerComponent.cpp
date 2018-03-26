#include "pch.h"
#include "ToolHandlerComponent.h"
#include "CurveEditorController.h"
#include "CurveEditorTool.h"

CCurveEditorToolHandlerComponent::CCurveEditorToolHandlerComponent(const CCurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView)
{
    m_ButtonHandlers.reserve(5);
    for (auto i = 0u; i < 5u; ++i)
        m_ButtonHandlers.emplace_back(*this, i);
}

void CCurveEditorToolHandlerComponent::OnFrame(ImDrawList&, CCurveEditorController& editorController)
{
    CaptureMouseState();
    UpdateMouseState(editorController);
    ReleaseMouseState();
}

void CCurveEditorToolHandlerComponent::CaptureMouseState()
{
    auto& io = ImGui::GetIO();

    m_MousePositionBuffer = io.MousePos;

    const auto& editorView = GetEditorView();
    for(auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.OnCapture(editorView);
}

void CCurveEditorToolHandlerComponent::UpdateMouseState(CCurveEditorController& editorController)
{
    const auto& activeTool = editorController.GetActiveTool();
    if (!activeTool)
        return;

    for (auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.Update(*activeTool);
}

void CCurveEditorToolHandlerComponent::ReleaseMouseState()
{
    auto& io = ImGui::GetIO();

    for (auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.OnRelease();

    io.MousePos = m_MousePositionBuffer;
}

CMouseButtonHandler::CMouseButtonHandler(CCurveEditorToolHandlerComponent& toolHandlerComponent, MouseButtonIndex buttonIndex) :
    m_ToolHandlerComponent(toolHandlerComponent),
    m_ButtonIndex(std::move(buttonIndex))
{
}

void CMouseButtonHandler::OnCapture(const CCurveEditorView& editorView)
{
    auto& io = ImGui::GetIO();
    const auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    m_ClickPositionBuffer = io.MouseClickedPos[m_ButtonIndex];
    io.MouseClickedPos[m_ButtonIndex] = windowCanvas.FromScreen(m_ClickPositionBuffer);
}

void CMouseButtonHandler::Update(ICurveEditorTool& activeTool)
{
    const auto wasDragging = m_IsDragging;
    const auto imGuiButtonIndex = static_cast<int>(m_ButtonIndex);
    m_IsDragging = ImGui::IsMouseDragging(imGuiButtonIndex, 1.0f);

    if (wasDragging != m_IsDragging)
    {
        if (m_IsDragging)
            activeTool.OnDragBegin(/*TODO*/);
        else
            activeTool.OnDragEnd(/*TODO*/);
    }
    else if (m_IsDragging)
        activeTool.OnDragUpdate(/*TODO*/);

    if (ImGui::IsMouseClicked(imGuiButtonIndex))
        activeTool.OnClick(/*TODO*/);

    if (ImGui::IsMouseDoubleClicked(imGuiButtonIndex))
        activeTool.OnDoubleClick(/*TODO*/);
}

void CMouseButtonHandler::OnRelease()
{
    auto& io = ImGui::GetIO();

    io.MouseClickedPos[m_ButtonIndex] = m_ClickPositionBuffer;
}