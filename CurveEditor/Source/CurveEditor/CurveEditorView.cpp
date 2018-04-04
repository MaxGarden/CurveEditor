#include "pch.h"
#include "CurveEditorView.h"
#include "CurveEditorViewComponent.h"
#include "CurveEditorDataModel.h"
#include "CurveEditorListenerBase.h"
#include "SplineViewFactory.h"
#include "SplineController.h"
#include <ImGuiInterop.h>

using namespace ImGuiInterop;
using namespace ax::ImGuiInterop;

class CCurveEditorViewListener final : public CCurveEditorListenerBase
{
public:
    CCurveEditorViewListener(CCurveEditorView& curveEditorView);
    virtual ~CCurveEditorViewListener() override final = default;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) override final;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) override final;

private:
    CCurveEditorView& m_CurveEditorView;
};

CCurveEditorViewListener::CCurveEditorViewListener(CCurveEditorView& curveEditorView) :
    m_CurveEditorView(curveEditorView)
{
}

void CCurveEditorViewListener::OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    const auto result = m_CurveEditorView.CreateSplineView(splineController);
    EDITOR_ASSERT(result);
}

void CCurveEditorViewListener::OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    const auto result = m_CurveEditorView.DestroySplineView(splineController);
    EDITOR_ASSERT(result);
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

    SetWindowCanvas();
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
    ApplyCanvas();

    ImGui::End();
}

bool CCurveEditorView::SetController(const IEditorControllerSharedPtr& controller) noexcept
{
    const auto previousController = GetController();

    if (!Super::SetController(controller))
        return false;

     if (previousController)
        previousController->UnregisterListener(m_ListenerHandle);

    if (const auto controller = GetController())
        m_ListenerHandle = controller->RegisterListener(std::make_unique<CCurveEditorViewListener>(*this)).value_or(0);

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

bool CCurveEditorView::AddView(IEditorViewUniquePtr&& view)
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

void CCurveEditorView::VisitViews(const VisitorType<IEditorView>& visitor) noexcept
{
    VisitContainer(m_Views, visitor);
}

void CCurveEditorView::VisitSplineViews(const VisitorType<ICurveEditorSplineView>& visitor) noexcept
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

    const auto controller = GetController();
    if (!controller)
        return;

    controller->VisitSplineControllers([this](const auto& controller)
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

void CCurveEditorView::SetWindowCanvas()
{
    auto& windowCanvas = GetCanvas().GetWindowCanvas();

    windowCanvas = CWindowCanvas{ to_pointf(ImGui::GetWindowPos()), to_sizef(ImGui::GetWindowSize()), ax::pointf{ 1.0f, 1.0f }, {} };
}

void CCurveEditorView::ApplyCanvas()
{
    const auto drawList = ImGui::GetWindowDrawList();
    EDITOR_ASSERT(drawList);
    if (!drawList)
        return;

    const auto& windowCanvas = GetCanvas().GetWindowCanvas();

    const auto windowScreenPosition = to_imvec(windowCanvas.GetWindowScreenPosition());
    const auto windowSize = to_imvec(windowCanvas.GetWindowScreenSize());
    const auto clientOrigin = to_imvec(windowCanvas.GetClientOrigin());

    const auto preOffset = ImVec2{ 0, 0 };
    const auto postOffset = windowScreenPosition + clientOrigin;
    const auto scale = ImVec2{ 1.0f, 1.0f };

    const auto backgroundChannelStart = ImGuiUtilities::GetBackgroundChannelStart();

    ImGuiUtilities::TransformDrawListChannels(*drawList, 0, 1, preOffset, scale, postOffset);
    ImGuiUtilities::TransformDrawListChannels(*drawList, backgroundChannelStart, drawList->_ChannelsCount - 1, preOffset, scale, postOffset);

    const auto windowScreenPositionPoint = windowCanvas.GetWindowScreenPosition();
    auto clipTranslation = to_imvec(windowScreenPositionPoint - windowCanvas.FromScreen(windowScreenPositionPoint));

    ImGui::PushClipRect(windowScreenPosition, windowScreenPosition + windowSize, false);
    ImGuiUtilities::TranslateAndClampDrawListClipRects(*drawList, 0, 1, clipTranslation);
    ImGuiUtilities::TranslateAndClampDrawListClipRects(*drawList, backgroundChannelStart, drawList->_ChannelsCount - 1, clipTranslation);
    ImGui::PopClipRect();
}