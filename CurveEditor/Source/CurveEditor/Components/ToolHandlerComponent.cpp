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
    CCurveEditorViewComponentBase(editorView)
{
    static_assert(g_MouseButtons.size() == static_cast<size_t>(ECurveEditorMouseButton::__Count));

    m_ButtonHandlers.reserve(g_MouseButtons.size());
    for(const auto& button : g_MouseButtons)
        m_ButtonHandlers.emplace_back(editorView, *this, button);
}

void CCurveEditorToolHandlerComponent::OnFrame()
{
    const auto& editorController = GetEditorView().GetController();
    EDITOR_ASSERT(editorController);
    if (!editorController)
        return;

    OnFrame(*editorController);
}

void CCurveEditorToolHandlerComponent::OnFrame(ICurveEditorController& editorController)
{
    UpdateActivity(editorController);
    if (!m_IsActive)
        return;

    CaptureMouseState();
    UpdateMouseState(editorController);
    ReleaseMouseState();
}

void CCurveEditorToolHandlerComponent::UpdateActivity(ICurveEditorController& editorController)
{
    const auto wasActive = m_IsActive;
    m_IsActive = ImGui::IsWindowFocused();

    if (!m_IsActive || wasActive == m_IsActive)
        return;

    if (const auto& activeTool = editorController.GetActiveTool())
        activeTool->OnActiveEditorViewChanged(CCurveEditorToolEvent{ GetEditorView() });
}

void CCurveEditorToolHandlerComponent::CaptureMouseState()
{
    auto& io = ImGui::GetIO();

    const auto& windowCanvas = GetEditorView().GetCanvas().GetWindowCanvas();

    for(auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.OnCapture();

    m_MousePositionBuffer = io.MousePos;
    io.MousePos = to_imvec(windowCanvas.FromScreen(to_pointf(m_MousePositionBuffer)));
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

    io.MousePos = m_MousePositionBuffer;

    for (auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.OnRelease();
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

    activeTool.OnWheel(CCurveEditorToolMouseWheelEvent{ GetEditorView(), to_pointf(ImGui::GetMousePos()), mouseWheelValue });
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

    const auto imGuiButtonIndex = GetButtonIndex();

    if(m_IsDragging = ImGui::IsMouseDragging(static_cast<int>(imGuiButtonIndex), 1.0f))
        m_DragDelta = to_pointf(ImGui::GetMouseDragDelta(static_cast<int>(imGuiButtonIndex), 1.0f));

    m_ClickPositionBuffer = io.MouseClickedPos[imGuiButtonIndex];
    io.MouseClickedPos[imGuiButtonIndex] = to_imvec(windowCanvas.FromScreen(to_pointf(m_ClickPositionBuffer)));
}

void CMouseButtonHandler::Update(ICurveEditorTool& activeTool)
{
    const auto imGuiButtonIndex = static_cast<int>(m_Button);
    const auto mousePosition = to_pointf(ImGui::GetMousePos());

    const auto notifyToolButtonEvent = [this, &activeTool, &mousePosition](const auto& method)
    {
        (activeTool.*method)(CCurveEditorToolMouseButtonEvent{ m_EditorView, mousePosition, m_Button });
    };

    const auto notifyToolDragEvent = [this, &activeTool, &mousePosition](const auto& method, const auto& currentDragDelta)
    {
        (activeTool.*method)(CCurveEditorToolMouseDragEvent{ m_EditorView, mousePosition, m_Button, currentDragDelta });
    };

    if (m_WasDragging != m_IsDragging)
    {
        if (m_IsDragging)
            notifyToolButtonEvent(&ICurveEditorTool::OnDragBegin);
        else
        {
            notifyToolButtonEvent(&ICurveEditorTool::OnDragEnd);
            m_DragDelta = {};
        }

        m_WasDragging = m_IsDragging;
    }
    else if (m_IsDragging)
    {
        const auto& windowCanvas = m_EditorView.GetCanvas().GetWindowCanvas();
        const auto currentDragDelta = m_DragDelta.cwise_product(windowCanvas.GetZoom());

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