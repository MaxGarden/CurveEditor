#include "pch.h"
#include "CurveEditorView.h"
#include "CurveEditorViewComponent.h"
#include "CurveEditorDataModel.h"
#include "CurveEditorProtocolBase.h"
#include "SplineViewFactory.h"
#include "SplineController.h"

class CCurveEditorViewProtocol final : public CCurveEditorProtocolBase
{
public:
    CCurveEditorViewProtocol(CCurveEditorView& curveEditorView);
    virtual ~CCurveEditorViewProtocol() override final = default;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) override final;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) override final;

private:
    CCurveEditorView& m_CurveEditorView;
};

CCurveEditorViewProtocol::CCurveEditorViewProtocol(CCurveEditorView& curveEditorView) :
    m_CurveEditorView(curveEditorView)
{
}

void CCurveEditorViewProtocol::OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    const auto result = m_CurveEditorView.CreateSplineView(splineController);
    EDITOR_ASSERT(result);
}

void CCurveEditorViewProtocol::OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    const auto result = m_CurveEditorView.DestroySplineView(splineController);
    EDITOR_ASSERT(result);
}

void CCurveEditorViewBase::OnFrame()
{
    //to override
}

bool CCurveEditorViewBase::SetController(const IEditorControllerSharedPtr& controller) noexcept
{
    if (!controller)
    {
        m_Controller.reset();
        OnControllerChanged();
        return true;
    }

    const auto curveEditorController = std::dynamic_pointer_cast<CCurveEditorViewController>(controller);
    if (!curveEditorController)
        return false;

    m_Controller = std::move(curveEditorController);
    OnControllerChanged();

    return true;
}

const CCurveEditorViewControllerSharedPtr& CCurveEditorViewBase::GetController() const noexcept
{
    return m_Controller;
}

void CCurveEditorViewBase::OnControllerChanged() noexcept
{
    //to override
}

CCurveEditorView::CCurveEditorView(ICurveEditorSplineViewFactory& splineViewFactory) :
    m_SplineViewFactory(splineViewFactory)
{
}

CCurveEditorView::~CCurveEditorView()
{
    SetController(nullptr);
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

    //only for tests
    //begin
    m_Canvas.GetWindowCanvas() = CWindowCanvas(ImGui::GetWindowPos(), ImGui::GetWindowSize(), ImVec2(1, 1), ImGui::GetWindowSize());
    //end
}

void CCurveEditorView::OnFrame()
{
    OnFrameBegin();

    VisitViews([](auto& view)
    {
        view.OnFrame();
    });

    VisitSplineViews([](auto& splineView)
    {
        splineView.OnFrame();
    });

    OnFrameEnd();
}

void CCurveEditorView::OnFrameEnd()
{
    ImGui::End();
}

bool CCurveEditorView::SetController(const IEditorControllerSharedPtr& controller) noexcept
{
    const auto previousController = GetController();

    if (!CCurveEditorViewBase::SetController(controller))
        return false;

     if (previousController)
        previousController->UnregisterProtocol(m_ProtocolHandle);

    if (const auto controller = GetController())
        m_ProtocolHandle = controller->RegisterProtocol(std::make_unique<CCurveEditorViewProtocol>(*this)).value_or(0);

    RecreateSplineViews();
    return true;
}

bool CCurveEditorView::CreateSplineView(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    if (m_SplineViews.find(splineController) != m_SplineViews.end())
    {
        EDITOR_ASSERT(false && "This controller already has a view");
        return false;
    }

    auto splineView = m_SplineViewFactory.Create(*this, splineController);
    if (!splineView)
        return false;

    auto isValid = true;
    isValid &= splineView->SetController(splineController);

    if (!isValid)
        return false;

    m_SplineViews.emplace(splineController, std::move(splineView));
    return true;
}

bool CCurveEditorView::DestroySplineView(const ICurveEditorSplineControllerConstSharedPtr& splineController)
{
    const auto iterator = m_SplineViews.find(splineController);
    if (iterator == m_SplineViews.end())
        return false;

    m_SplineViews.erase(iterator);
    return true;
}

CEditorCanvas& CCurveEditorView::GetCanvas() noexcept
{
    return m_Canvas;
}

const CEditorCanvas& CCurveEditorView::GetCanvas() const noexcept
{
    return m_Canvas;
}

bool CCurveEditorView::AddView(CCurveEditorViewBaseUniquePtr&& view)
{
    if (!view)
        return false;

    auto isValid = true;
    isValid &= view->SetController(GetController());

    if (!isValid)
        return false;

    m_Views.emplace_back(std::move(view));
    return true;
}

void CCurveEditorView::VisitViews(const std::function<void(CCurveEditorViewBase&)>& visitor) noexcept
{
    if (!visitor)
        return;

    for (const auto& view : m_Views)
    {
        if (view)
            visitor(*view);
    }
}

void CCurveEditorView::VisitSplineViews(const std::function<void(ICurveEditorSplineView&)>& visitor) noexcept
{
    if (!visitor)
        return;

    for (const auto& splineViewPair : m_SplineViews)
    {
        if (const auto& splineView = splineViewPair.second)
            visitor(*splineView);
    }
}

void CCurveEditorView::RecreateSplineViews()
{
    std::vector<ICurveEditorSplineControllerConstSharedPtr> splineControllersToDestroy;
    std::transform(m_SplineViews.begin(), m_SplineViews.end(), std::back_inserter(splineControllersToDestroy),
        [](const auto& pair)
        {
            return pair.first;
        });

    for (const auto& splineController : splineControllersToDestroy)
        DestroySplineView(splineController);

    const auto viewController = GetController();
    if (!viewController)
        return;

    auto& editorController = viewController->GetEditorController();

    editorController.VisitSplineControllers([this](const auto& controller)
    {
        CreateSplineView(controller);
    });
}

void CCurveEditorView::OnControllerChanged() noexcept
{
    auto result = true;
    const auto& controller = GetController();

    VisitViews([&controller, &result](auto& view)
    {
        result &= view.SetController(controller);
    });

    EDITOR_ASSERT(result && "Views should accept new controller if main view accepts.");

    RecreateSplineViews();
}