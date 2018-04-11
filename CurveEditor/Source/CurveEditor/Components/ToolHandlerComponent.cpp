#include "pch.h"
#include "ToolHandlerComponent.h"
#include "CurveEditorController.h"
#include "CurveEditorTool.h"
#include <ImGuiInterop.h>

using namespace ImGuiInterop;
using namespace ax::ImGuiInterop;

static constexpr std::array<ECurveEditorMouseButton, 5> g_MouseButtons = {ECurveEditorMouseButton::Left,
                                                                          ECurveEditorMouseButton::Right,
                                                                          ECurveEditorMouseButton::Middle,
                                                                          ECurveEditorMouseButton::User1,
                                                                          ECurveEditorMouseButton::User2};

CCurveEditorToolHandlerComponent::CCurveEditorToolHandlerComponent(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
    static_assert(g_MouseButtons.size() == static_cast<size_t>(ECurveEditorMouseButton::__Count));

    m_ButtonHandlers.reserve(g_MouseButtons.size());
    for(const auto& button : g_MouseButtons)
        m_ButtonHandlers.emplace_back(editorView, *this, button);
}

void CCurveEditorToolHandlerComponent::OnFrame()
{
    const auto editorController = m_EditorView.GetController();
    EDITOR_ASSERT(editorController);
    if (!editorController)
        return;

    OnFrame(*editorController);
}

void CCurveEditorToolHandlerComponent::OnFrame(ICurveEditorController& editorController)
{
    CaptureMouseState();
    UpdateMouseState(editorController);
    ReleaseMouseState();
}

void CCurveEditorToolHandlerComponent::CaptureMouseState()
{
    auto& io = ImGui::GetIO();

    const auto& windowCanvas = m_EditorView.GetCanvas().GetWindowCanvas();

    m_MousePositionBuffer = io.MousePos;
    io.MousePos = to_imvec(windowCanvas.FromScreen(to_pointf(m_MousePositionBuffer)));

    for(auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.OnCapture();
}

void CCurveEditorToolHandlerComponent::UpdateMouseState(ICurveEditorController& editorController)
{
    const auto& activeTool = editorController.GetActiveTool();
    if (!activeTool)
        return;

    for (auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.Update(*activeTool);

    UpdateWheelState(*activeTool);
}

void CCurveEditorToolHandlerComponent::ReleaseMouseState()
{
    auto& io = ImGui::GetIO();

    for (auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.OnRelease();

    io.MousePos = m_MousePositionBuffer;
}

void CCurveEditorToolHandlerComponent::VisitButtonHandlers(const ConstVisitorType<CMouseButtonHandler>& visitor) const noexcept
{
    VisitObjectsContainer(m_ButtonHandlers, visitor);
}

void CCurveEditorToolHandlerComponent::UpdateWheelState(ICurveEditorTool& activeTool)
{
    auto& io = ImGui::GetIO();
    const auto mouseWheelValue = io.MouseWheel;

    if (mouseWheelValue == 0.0f)
        return;

    activeTool.OnWheel(CCurveEditorToolMouseWheelEvent(m_EditorView, to_pointf(ImGui::GetMousePos()), mouseWheelValue));
}

CMouseButtonHandler::CMouseButtonHandler(ICurveEditorView& editorView, const CCurveEditorToolHandlerComponent& toolHandler, ECurveEditorMouseButton button) :
    m_EditorView(editorView),
    m_ToolHandler(toolHandler),
    m_Button(button)
{
}

void CMouseButtonHandler::OnCapture()
{
    auto& io = ImGui::GetIO();
    const auto& windowCanvas = m_EditorView.GetCanvas().GetWindowCanvas();

    const auto buttonIndex = GetButtonIndex();

    m_ClickPositionBuffer = io.MouseClickedPos[buttonIndex];
    io.MouseClickedPos[buttonIndex] = to_imvec(windowCanvas.FromScreen(to_pointf(m_ClickPositionBuffer)));
}

void CMouseButtonHandler::Update(ICurveEditorTool& activeTool)
{
    const auto wasDragging = m_IsDragging;
    const auto imGuiButtonIndex = static_cast<int>(m_Button);

    const auto notifyToolButtonEvent = [this, &activeTool](const auto& method)
    {
        (activeTool.*method)(CCurveEditorToolMouseButtonEvent(m_EditorView, to_pointf(ImGui::GetMousePos()), m_Button));
    };

    const auto notifyToolDragEvent = [this, &activeTool](const auto& method, const auto& currentDragDelta)
    {
        (activeTool.*method)(CCurveEditorToolMouseDragEvent(m_EditorView, to_pointf(ImGui::GetMousePos()), m_Button, to_pointf(currentDragDelta), to_pointf(m_TotalDragDelta)));
    };

    const auto canStartDrag = [this]()
    {
        bool result = true;

        m_ToolHandler.VisitButtonHandlers([this, &result](const auto& buttonHandler)
        {
            EDITOR_ASSERT(GetButton() != buttonHandler.GetButton() || this == &buttonHandler);

            if (this != &buttonHandler && buttonHandler.IsDragging())
                result = false;
        });

        return result;
    };

    m_IsDragging = ImGui::IsMouseDragging(imGuiButtonIndex, 1.0f) && (m_IsDragging || canStartDrag());

    if (wasDragging != m_IsDragging)
    {
        if (m_IsDragging)
            notifyToolDragEvent(&ICurveEditorTool::OnDragBegin, ImVec2{});
        else
        {
            notifyToolDragEvent(&ICurveEditorTool::OnDragEnd, ImVec2{});
            m_TotalDragDelta = {};
        }
    }
    else if (m_IsDragging)
    {
        const auto mouseDragDelta = ImGui::GetMouseDragDelta(imGuiButtonIndex);
        const auto currentDragDelta = mouseDragDelta - m_TotalDragDelta;
        m_TotalDragDelta = mouseDragDelta;

        notifyToolDragEvent(&ICurveEditorTool::OnDragUpdate, currentDragDelta);
    }
    else if (ImGui::IsMouseClicked(imGuiButtonIndex))
        notifyToolButtonEvent(&ICurveEditorTool::OnClick);
    else if (ImGui::IsMouseDoubleClicked(imGuiButtonIndex))
        notifyToolButtonEvent(&ICurveEditorTool::OnDoubleClick);
}

void CMouseButtonHandler::OnRelease()
{
    auto& io = ImGui::GetIO();

    const auto buttonIndex = GetButtonIndex();
    io.MouseClickedPos[buttonIndex] = m_ClickPositionBuffer;
}

bool CMouseButtonHandler::IsDragging() const noexcept
{
    return m_IsDragging;
}

ECurveEditorMouseButton CMouseButtonHandler::GetButton() const noexcept
{
    return m_Button;
}

size_t CMouseButtonHandler::GetButtonIndex() const noexcept
{
    return static_cast<size_t>(m_Button);
}