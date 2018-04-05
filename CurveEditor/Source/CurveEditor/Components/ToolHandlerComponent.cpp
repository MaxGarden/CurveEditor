#include "pch.h"
#include "ToolHandlerComponent.h"
#include "CurveEditorController.h"
#include "CurveEditorTool.h"
#include <ImGuiInterop.h>

using namespace ax::ImGuiInterop;

static constexpr std::array<ECurveEditorMouseButton, 5> g_MouseButtons = {ECurveEditorMouseButton::Left,
                                                                          ECurveEditorMouseButton::Right,
                                                                          ECurveEditorMouseButton::Middle,
                                                                          ECurveEditorMouseButton::User1,
                                                                          ECurveEditorMouseButton::User2};

CCurveEditorToolHandlerComponent::CCurveEditorToolHandlerComponent(const CCurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView)
{
    static_assert(g_MouseButtons.size() == static_cast<size_t>(ECurveEditorMouseButton::__Count));

    m_ButtonHandlers.reserve(g_MouseButtons.size());
    for(const auto& button : g_MouseButtons)
        m_ButtonHandlers.emplace_back(editorView, *this, button);
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

    for(auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.OnCapture();
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

void CCurveEditorToolHandlerComponent::VisitButtonHandlers(const ConstVisitorType<CMouseButtonHandler>& visitor) const noexcept
{
    VisitObjectsContainer(m_ButtonHandlers, visitor);
}

CMouseButtonHandler::CMouseButtonHandler(const CCurveEditorView& editorView, const CCurveEditorToolHandlerComponent& toolHandler, ECurveEditorMouseButton button) :
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

    const auto notifyTool = [this, &activeTool](const auto& method)
    {
        (activeTool.*method)(CCurveEditorToolMouseButtonEvent(m_EditorView, to_pointf(ImGui::GetMousePos()), m_Button));
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
            notifyTool(&ICurveEditorTool::OnDragBegin);
        else
            notifyTool(&ICurveEditorTool::OnDragEnd);
    }
    else if (m_IsDragging)
        notifyTool(&ICurveEditorTool::OnDragUpdate);
    else if (ImGui::IsMouseClicked(imGuiButtonIndex))
        notifyTool(&ICurveEditorTool::OnClick);
    else if (ImGui::IsMouseDoubleClicked(imGuiButtonIndex))
        notifyTool(&ICurveEditorTool::OnDoubleClick);
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