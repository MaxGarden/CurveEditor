#include "pch.h"
#include "CurveEditorToolEvent.h"
#include "ToolHandlerComponent.h"
#include "CurveEditorViewComponentBase.h"
#include "CurveEditorController.h"
#include "CurveEditorTool.h"
#include <ImGuiInterop.h>
#include <mutex>

using namespace ImGuiInterop;
using namespace ax::ImGuiInterop;

static constexpr std::array<ECurveEditorMouseButton, 5> g_MouseButtons = {ECurveEditorMouseButton::Left,
                                                                          ECurveEditorMouseButton::Right,
                                                                          ECurveEditorMouseButton::Middle,
                                                                          ECurveEditorMouseButton::User1,
                                                                          ECurveEditorMouseButton::User2};

class CCurveEditorToolHandlerComponent;

class CMouseButtonHandler
{
public:
    CMouseButtonHandler(ICurveEditorView& editorView, const CCurveEditorToolHandlerComponent& toolHandler, ECurveEditorMouseButton button);
    virtual ~CMouseButtonHandler() = default;

    void OnCapture();
    void Update(ICurveEditorTool& activeTool);
    void OnRelease();

    void ForceStopWorking(ICurveEditorTool* activeTool);

    bool IsWorking() const noexcept;
    bool IsDragging() const noexcept;
    ECurveEditorMouseButton GetButton() const noexcept;

private:
    size_t GetButtonIndex() const noexcept;
    void ForceStopDragging(ICurveEditorTool* activeTool);

private:
    ICurveEditorView & m_EditorView;
    const CCurveEditorToolHandlerComponent& m_ToolHandler;
    bool m_IsDragging = false;
    bool m_WasDragging = false;
    ECurveEditorMouseButton m_Button;
    ImVec2 m_ClickPositionBuffer;
    ax::pointf m_DragDelta;
    ax::pointf m_LastDragDelta;
};

class CCurveEditorToolHandlerComponent final : public CCurveEditorViewComponentBase<ICurveEditorToolHandlerComponent>
{
public:
    CCurveEditorToolHandlerComponent(ICurveEditorView& editorView);
    virtual ~CCurveEditorToolHandlerComponent() override final;

    virtual void OnFrame() override final;

    void VisitButtonHandlers(const ConstVisitorType<CMouseButtonHandler>& visitor) const noexcept;

protected:
    virtual void OnControllerChanged() override final;
    void OnFrame(ICurveEditorController& editorController);

private:
    void UpdateActivity(ICurveEditorController& editorController, bool shouldBeActive);
    bool TryAcquireActivity();
    bool TryReleaseActivity();
    void CaptureMouseState();
    void UpdateMouseState(ICurveEditorController& editorController);
    void UpdateWheelState(ICurveEditorTool& activeTool);
    void UpdateMouseMoveState(ICurveEditorTool& activeTool);
    void ReleaseMouseState();

    void ForceStopWorkingButtonHandlers(ICurveEditorTool* activeTool);
    void ResetActivity(ICurveEditorController* controller);

private:
    ImVec2 m_LastMousePosition;
    ImVec2 m_MousePositionBuffer;
    ImVec2 m_MouseClicksPositionBuffers[5];
    std::vector<CMouseButtonHandler> m_ButtonHandlers;
    bool m_IsActive = false;
    ICurveEditorControllerWeakPtr m_LastController;

    static std::mutex s_ActivityMutex;
    static CCurveEditorToolHandlerComponent* s_ActivityOwner;
};

std::mutex CCurveEditorToolHandlerComponent::s_ActivityMutex;
CCurveEditorToolHandlerComponent* CCurveEditorToolHandlerComponent::s_ActivityOwner = nullptr;

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
    const auto& editorController = GetController();
    EDITOR_ASSERT(editorController);
    if (!editorController)
        return;

    OnFrame(*editorController);
}

void CCurveEditorToolHandlerComponent::OnFrame(ICurveEditorController& editorController)
{
    UpdateActivity(editorController, ImGui::IsWindowHovered());

    if (!m_IsActive)
        return;

    CaptureMouseState();
    UpdateMouseState(editorController);
    ReleaseMouseState();
}

void CCurveEditorToolHandlerComponent::UpdateActivity(ICurveEditorController& editorController, bool shouldBeActive)
{
    const auto& activeTool = editorController.GetActiveTool();

    if (m_IsActive && !activeTool)
    {
        ForceStopWorkingButtonHandlers(nullptr);
        shouldBeActive = false;
    }

    if (!activeTool || shouldBeActive == m_IsActive)
        return;

    if (shouldBeActive)
    {
        if (!TryAcquireActivity())
            return;
    }
    else
    {
        if (!TryReleaseActivity())
            return;
    }

    if (!activeTool)
        return;

    if (m_IsActive)
        activeTool->OnAcquired(CCurveEditorToolEvent{ GetEditorView() });
    else
        activeTool->OnReleased(CCurveEditorToolEvent{ GetEditorView() });
}

bool CCurveEditorToolHandlerComponent::TryAcquireActivity()
{
    EDITOR_ASSERT(!m_IsActive);
    if (m_IsActive)
        return true;

    std::lock_guard<std::mutex> guard(s_ActivityMutex);

    if (s_ActivityOwner)
        return false;

    s_ActivityOwner = this;
    return (m_IsActive = true);
}

bool CCurveEditorToolHandlerComponent::TryReleaseActivity()
{
    EDITOR_ASSERT(m_IsActive);
    if (!m_IsActive)
        return true;

    std::lock_guard<std::mutex> guard(s_ActivityMutex);

    EDITOR_ASSERT(s_ActivityOwner == this);
    if (s_ActivityOwner != this)
        return false;

    for (const auto& buttonHandler : m_ButtonHandlers)
    {
        if (buttonHandler.IsWorking())
            return false;
    }

    s_ActivityOwner = nullptr;
    m_IsActive = false;
    return true;
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

    UpdateMouseMoveState(*activeTool);
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

void CCurveEditorToolHandlerComponent::UpdateMouseMoveState(ICurveEditorTool& activeTool)
{
    const auto mousePosition = ImGui::GetMousePos();

    if (mousePosition != m_LastMousePosition)
        activeTool.OnMouseMove(CCurveEditorToolMouseEvent{ GetEditorView(), to_pointf(mousePosition) });

    m_LastMousePosition = mousePosition;
}

CCurveEditorToolHandlerComponent::~CCurveEditorToolHandlerComponent()
{
    ResetActivity(GetController().get());
}

void CCurveEditorToolHandlerComponent::ForceStopWorkingButtonHandlers(ICurveEditorTool* activeTool)
{
    for (auto& buttonHandler : m_ButtonHandlers)
        buttonHandler.ForceStopWorking(activeTool);
}

void CCurveEditorToolHandlerComponent::OnControllerChanged()
{
    ResetActivity(m_LastController.lock().get());
    m_LastController = GetController();
}

void CCurveEditorToolHandlerComponent::ResetActivity(ICurveEditorController* controller)
{
    if (m_IsActive)
    {
        if (controller)
            UpdateActivity(*controller, false);
        else
        {
            ForceStopWorkingButtonHandlers(nullptr);
            TryReleaseActivity();
        }

        EDITOR_ASSERT(!m_IsActive);
    }
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
            m_DragDelta = m_LastDragDelta = {};
        }

        m_WasDragging = m_IsDragging;
    }
    else if (m_IsDragging)
    {
        const auto& windowCanvas = m_EditorView.GetCanvas().GetWindowCanvas();
        const auto currentDragDelta = m_DragDelta.cwise_product(windowCanvas.GetZoom());

        if(currentDragDelta != m_LastDragDelta)
            notifyToolDragEvent(&ICurveEditorTool::OnDragUpdate, currentDragDelta);

        m_LastDragDelta = currentDragDelta;
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

void CMouseButtonHandler::ForceStopWorking(ICurveEditorTool* activeTool)
{
    if (IsWorking())
        return;

    if (IsDragging())
        ForceStopDragging(activeTool);
}

bool CMouseButtonHandler::IsWorking() const noexcept
{
    return IsDragging();
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

void CMouseButtonHandler::ForceStopDragging(ICurveEditorTool* activeTool)
{
    if (!IsDragging())
        return;

    m_IsDragging = false;
    if(activeTool)
        Update(*activeTool);
}

ICurveEditorToolHandlerComponentUniquePtr ICurveEditorToolHandlerComponent::Create(ICurveEditorView& editorView)
{
    return std::make_unique<CCurveEditorToolHandlerComponent>(editorView);
}