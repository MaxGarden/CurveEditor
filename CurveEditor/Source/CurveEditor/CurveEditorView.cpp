#include "pch.h"
#include "CurveEditorView.h"
#include "CurveEditorViewComponent.h"
#include "CurveEditorDataModel.h"
#include <ImGuiInterop.h>

using namespace ImGuiInterop;
using namespace ax::ImGuiInterop;

struct SComponentStorage
{
    SComponentStorage(EComponentOrder order, ICurveEditorViewComponentUniquePtr&& viewComponent);

    operator bool() const noexcept;
    ICurveEditorViewComponent& operator*() const;

    bool operator==(void* pointer) const noexcept;

    bool operator<(SComponentStorage& rhs) const noexcept;

    EComponentOrder Order;
    ICurveEditorViewComponentSharedPtr ViewComponent;
};

SComponentStorage::SComponentStorage(EComponentOrder order, ICurveEditorViewComponentUniquePtr&& viewComponent) :
    Order(order),
    ViewComponent(std::move(viewComponent))
{
}

SComponentStorage::operator bool() const noexcept
{
    return ViewComponent != nullptr;
}

ICurveEditorViewComponent& SComponentStorage::operator*() const
{
    return *ViewComponent;
}

bool SComponentStorage::operator==(void* pointer) const noexcept
{
    return ViewComponent.get() == pointer;
}

bool SComponentStorage::operator<(SComponentStorage& rhs) const noexcept
{
    return static_cast<size_t>(Order) < static_cast<size_t>(rhs.Order);
}

class CCurveEditorView final : public CEditorViewBase<ICurveEditorView, ICurveEditorController>
{
public:
    CCurveEditorView() = default;
    virtual ~CCurveEditorView();

    virtual bool Initialize() override final;

    virtual void OnFrame() override final;

    virtual CEditorCanvas& GetCanvas() noexcept override final;
    virtual const CEditorCanvas& GetCanvas() const noexcept override final;

    virtual std::optional<EditorViewComponentHandle> AddViewComponent(ICurveEditorViewComponentUniquePtr&& viewComponent, EComponentOrder order) override final;
    virtual bool RemoveViewComponent(const EditorViewComponentHandle& handle) override final;

    virtual ICurveEditorViewComponentSharedPtr GetViewComponent(const std::type_info& typeInfo) const noexcept override final;
    virtual ICurveEditorViewComponentSharedPtr GetViewComponent(const EditorViewComponentHandle& handle) const noexcept override final;

protected:
    virtual void OnControllerChanged() noexcept override final;

private:
    void VisitViewComponents(const VisitorType<ICurveEditorViewComponent>& visitor) noexcept;

    void OnFrameBegin();
    void OnFrameEnd();

    void RefreshWindowCanvas();

private:
    CEditorCanvas m_Canvas = CEditorCanvas({ 100.0f, 100.0f });

    std::vector<SComponentStorage> m_Components;

    bool m_Initialized = false;
};

CCurveEditorView::~CCurveEditorView()
{
    SetController(nullptr);
}

bool CCurveEditorView::Initialize()
{
    EDITOR_ASSERT(!m_Initialized);

    bool result = true;
    std::vector<void*> componentsToRemove;

    VisitViewComponents([&result, &componentsToRemove](auto& viewComponent)
    {
        if (viewComponent.Initialize())
            return;

        result = false;
        EDITOR_ASSERT(result && "Cannot initialize component");
        componentsToRemove.emplace_back(&viewComponent);
    });

    for (const auto& component : componentsToRemove)
        RemoveFromContainer(m_Components, component);

    m_Initialized = true;

    return result;
}

void CCurveEditorView::OnFrameBegin()
{
    static const auto editorWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    const auto& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("CurveEditorView", nullptr, ImVec2(0.0f, 0.0f), 0.0f, editorWindowFlags);

    RefreshWindowCanvas();
}

void CCurveEditorView::OnFrame()
{
    OnFrameBegin();

    VisitViewComponents([](auto& view)
    {
        view.OnFrame();
    });

    OnFrameEnd();
}

void CCurveEditorView::OnFrameEnd()
{
    ImGui::End();
}

CEditorCanvas& CCurveEditorView::GetCanvas() noexcept
{
    return m_Canvas;
}

const CEditorCanvas& CCurveEditorView::GetCanvas() const noexcept
{
    return m_Canvas;
}

std::optional<EditorViewComponentHandle> CCurveEditorView::AddViewComponent(ICurveEditorViewComponentUniquePtr&& viewComponent, EComponentOrder order)
{
    if (!viewComponent)
        return std::nullopt;

    auto isValid = true;
    isValid &= viewComponent->SetController(GetController());

    if (!isValid)
        return std::nullopt;

    if (m_Initialized)
    {
        const auto initialized = viewComponent->Initialize();
        EDITOR_ASSERT(initialized);

        if (!initialized)
            return std::nullopt;
    }

    m_Components.emplace_back(order, std::move(viewComponent));

    const auto handle = reinterpret_cast<EditorViewComponentHandle>(m_Components.back().ViewComponent.get());

    std::stable_sort(m_Components.begin(), m_Components.end());

    return handle;
}

bool CCurveEditorView::RemoveViewComponent(const EditorViewComponentHandle& handle)
{
    const auto iterator = std::find_if(m_Components.begin(), m_Components.end(), [&handle](const auto& componentStorage)
    {
        return reinterpret_cast<EditorViewComponentHandle>(componentStorage.ViewComponent.get()) == handle;
    });

    if (iterator == m_Components.end())
        return false;

    iterator->ViewComponent.reset();
    return true;
}

void CCurveEditorView::VisitViewComponents(const VisitorType<ICurveEditorViewComponent>& visitor) noexcept
{
    RemoveFromContainer(m_Components, nullptr);
    VisitPointersContainer(m_Components, visitor);
}

void CCurveEditorView::OnControllerChanged() noexcept
{
    auto result = true;
    const auto& controller = GetController();

    VisitViewComponents([&controller, &result](auto& view)
    {
        result &= view.SetController(controller);
    });

    EDITOR_ASSERT(result && "Views should accept new controller if main view accepts.");
}

void CCurveEditorView::RefreshWindowCanvas()
{
    auto& windowCanvas = GetCanvas().GetWindowCanvas();

    windowCanvas.SetWindowScreenPosition(to_pointf(ImGui::GetWindowPos()));
    windowCanvas.SetWindowScreenSize(to_sizef(ImGui::GetWindowSize()));
}

ICurveEditorViewComponentSharedPtr CCurveEditorView::GetViewComponent(const std::type_info& typeInfo) const noexcept
{
    for (const auto& storage : m_Components)
    {
        const auto& viewComponent = storage.ViewComponent;
        if (typeid(*viewComponent) == typeInfo)
            return viewComponent;
    }

    return nullptr;
}

ICurveEditorViewComponentSharedPtr CCurveEditorView::GetViewComponent(const EditorViewComponentHandle& handle) const noexcept
{
    const auto iterator = std::find(m_Components.begin(), m_Components.end(), reinterpret_cast<void*>(handle));
    if (iterator == m_Components.end())
        return nullptr;

    return iterator->ViewComponent;
}

ICurveEditorViewUniquePtr ICurveEditorView::Create()
{
    return std::make_unique<CCurveEditorView>();
}