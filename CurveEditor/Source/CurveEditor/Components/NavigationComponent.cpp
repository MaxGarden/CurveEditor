#include "pch.h"
#include "NavigationComponent.h"
#include "CurveEditorViewComponentBase.h"

class CCurveEditorNavigationComponent;

class CNavigationAnimationController final
{
public:
    CNavigationAnimationController(CCurveEditorNavigationComponent& navigationComponent);
    ~CNavigationAnimationController() = default;

    void Start(const CWindowCanvas& windowCanvas, const ax::pointf& targetOrigin, const ax::pointf& targetZoom, float duration, bool adjustZoomLevelAfterStop = true);
    void Stop();
    void OnFrame(ICurveEditorView& editorView);

private:
    void OnUpdate(ICurveEditorView& editorView, float progress);

private:
    CCurveEditorNavigationComponent& m_NavigationComponent;
    ax::pointf m_StartOrigin;
    ax::pointf m_StartZoom;
    ax::pointf m_OriginDelta;
    ax::pointf m_ZoomDelta;

    ax::pointf m_LastOrigin;
    ax::pointf m_LastZoom;

    float m_Time = 0.0f;
    float m_Duration = 0.0f;

    bool m_IsWorking = false;
    bool m_AdjustZoomLevelAfterStop = true;
};

class CCurveEditorNavigationComponent final : public CCurveEditorViewComponentBase<ICurveEditorNavigationComponent>
{
public:
    CCurveEditorNavigationComponent(ICurveEditorView& editorView);
    virtual ~CCurveEditorNavigationComponent() override final = default;

    virtual void OnFrame() override final;

    virtual void ApplyZoom(const ax::pointf& focusPoint, int steps, const ax::pointf& axisMultiplier, std::optional<float> zoomTimeOverride) override final;
    virtual void NavigateTo(const ax::rectf& boinds, std::optional<float> navigationTimeOverride = std::nullopt) override final;

    void AdjustZoomLevelToZoom(const ax::pointf& zoom);

private:
    ax::pointf m_ZoomLevel;
    CNavigationAnimationController m_AnimationController;
};

CNavigationAnimationController::CNavigationAnimationController(CCurveEditorNavigationComponent& navigationComponent) :
    m_NavigationComponent(navigationComponent)
{
}

void CNavigationAnimationController::Start(const CWindowCanvas& windowCanvas, const ax::pointf& targetOrigin, const ax::pointf& targetZoom, float duration, bool adjustZoomLevelAfterStop /* = true */)
{
    EDITOR_ASSERT(duration >= 0.0f);

    Stop();
    m_LastOrigin = m_StartOrigin = windowCanvas.GetClientOrigin();
    m_LastZoom = m_StartZoom = windowCanvas.GetZoom();
    m_OriginDelta = targetOrigin - m_StartOrigin;
    m_ZoomDelta = targetZoom - m_StartZoom;
    m_Duration = std::max(0.0f, duration);
    m_AdjustZoomLevelAfterStop = adjustZoomLevelAfterStop;

    m_IsWorking = true;
}

void CNavigationAnimationController::Stop()
{
    if (m_IsWorking && m_AdjustZoomLevelAfterStop)
        m_NavigationComponent.AdjustZoomLevelToZoom(m_LastZoom);

    m_Time = m_Duration = 0.0f;
    m_StartOrigin = {};
    m_StartZoom = {};
    m_OriginDelta = {};
    m_ZoomDelta = {};
    m_LastZoom = {};
    m_LastOrigin = {};

    m_IsWorking = m_AdjustZoomLevelAfterStop = false;
}

void CNavigationAnimationController::OnFrame(ICurveEditorView& editorView)
{
    if (!m_IsWorking)
        return;

    m_Time += std::max(0.0f, ImGui::GetIO().DeltaTime);

    if (m_Time < m_Duration)
        OnUpdate(editorView, m_Time / m_Duration);
    else
    {
        OnUpdate(editorView, 1.0f);
        Stop();
    }
}

void CNavigationAnimationController::OnUpdate(ICurveEditorView& editorView, float progress)
{
    static const auto interpolate = [](auto begin, auto delta, auto progress)
    {
        return begin - delta * progress * (progress - 2.0f);
    };

    auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    const auto& origin = windowCanvas.GetClientOrigin();
    const auto& zoom = windowCanvas.GetZoom();

    if (origin != m_LastOrigin || zoom != m_LastZoom)
    {
        Stop();
        return;
    }

    windowCanvas.SetClientOrigin(interpolate(m_StartOrigin, m_OriginDelta, progress));
    windowCanvas.SetZoom(interpolate(m_StartZoom, m_ZoomDelta, progress));

    m_LastOrigin = origin;
    m_LastZoom = zoom;
}

CCurveEditorNavigationComponent::CCurveEditorNavigationComponent(ICurveEditorView& editorView) :
    CCurveEditorViewComponentBase(editorView),
    m_AnimationController(*this)
{
}

void CCurveEditorNavigationComponent::OnFrame()
{
    m_AnimationController.OnFrame(GetEditorView());
}

void CCurveEditorNavigationComponent::ApplyZoom(const ax::pointf& focusPoint, int steps, const ax::pointf& axisMultiplier, std::optional<float> zoomTimeOverride)
{
    auto& editorView = GetEditorView();
    const auto& editorStyle = editorView.GetEditorStyle();

    auto applyAxisZoom = [&editorStyle](auto steps, auto& level, auto& zoom)
    {
        level += steps;
        level = std::max(editorStyle.MinimumZoomLevel, std::min(level, editorStyle.MaximumZoomLevel));

        if (level > 0)
            zoom = pow(editorStyle.ZoomInMultipler, level);
        else
            zoom = pow(editorStyle.ZoomOutMultipler, abs(level));
    };

    m_AnimationController.Stop();

    auto& windowCanvas = editorView.GetCanvas().GetWindowCanvas();

    auto targetZoom = windowCanvas.GetZoom();

    applyAxisZoom(steps * axisMultiplier.x, m_ZoomLevel.x, targetZoom.x);
    applyAxisZoom(steps * axisMultiplier.y, m_ZoomLevel.y, targetZoom.y);

    const auto screenPosition = windowCanvas.ToScreen(focusPoint);
    const auto previousCanvasPosition = windowCanvas.FromScreen(screenPosition);

    const auto startZoom = windowCanvas.GetZoom();

    windowCanvas.SetZoom(targetZoom);

    const auto canvasPosition = windowCanvas.FromScreen(screenPosition);
    const auto offset = ax::pointf{ (canvasPosition.x - previousCanvasPosition.x) * targetZoom.x, (canvasPosition.y - previousCanvasPosition.y) * targetZoom.y };

    const auto& startOrigin = windowCanvas.GetClientOrigin();
    const auto targetOrigin = startOrigin + offset;

    windowCanvas.SetZoom(startZoom);

    m_AnimationController.Start(windowCanvas, targetOrigin, targetZoom, zoomTimeOverride.value_or(editorStyle.ZoomDurationTime), false);
}

void CCurveEditorNavigationComponent::NavigateTo(const ax::rectf& bounds, std::optional<float> navigationTimeOverride /*= std::nullopt*/)
{
    const auto& editorView = GetEditorView();
    const auto& editorCanvas = editorView.GetCanvas();
    const auto& windowCanvas = editorCanvas.GetWindowCanvas();

    const auto originalOrigin = windowCanvas.GetClientOrigin();
    const auto originalZoom = windowCanvas.GetZoom();

    const auto& editorStyle = editorView.GetEditorStyle();

    const auto visibleBounds = windowCanvas.CalculateVisibleBounds();

    const auto& navigationZoomMargin = editorStyle.NavigationZoomMargin;

    auto newWindowCanvas = windowCanvas;
    auto ratio = ax::pointf{ 1.0f, 1.0f };

    if (bounds.w != 0.0f)
        ratio.x = ((1.0f - navigationZoomMargin) * visibleBounds.w / bounds.w);

    if (bounds.h != 0.0f)
        ratio.y = ((1.0f - navigationZoomMargin) * visibleBounds.h / bounds.h);

    const auto minimumZoom = pow(editorStyle.ZoomOutMultipler, abs(editorStyle.MinimumZoomLevel));
    const auto maximumZoom = pow(editorStyle.ZoomInMultipler, abs(editorStyle.MaximumZoomLevel));

    const auto& zoom = windowCanvas.GetZoom();
    newWindowCanvas.SetZoom(ax::pointf{ std::max(minimumZoom, std::min(zoom.x * ratio.x, maximumZoom)), std::max(minimumZoom, std::min(zoom.y * ratio.y, maximumZoom)) });

    const auto newVisibleBounds = newWindowCanvas.CalculateVisibleBounds().center();

    const auto& newZoom = newWindowCanvas.GetZoom();
    const auto offset = (newVisibleBounds - bounds.center()).cwise_product(newZoom);

    const auto& newOrigin = newWindowCanvas.GetClientOrigin();
    newWindowCanvas.SetClientOrigin(newOrigin + offset);

    const auto& origin = windowCanvas.GetClientOrigin();

    const auto originOffset = newOrigin - origin;
    const auto zoomOffset = newZoom - zoom;

    static const auto epsilon = 1e-4f;

    AdjustZoomLevelToZoom(newZoom);

    if (fabsf(originOffset.x) < epsilon && fabsf(originOffset.y) < epsilon && fabsf(zoomOffset.x) < epsilon && fabsf(zoomOffset.y) < epsilon)
        m_AnimationController.Start(windowCanvas, newOrigin, newZoom, 0.0f);
    else
        m_AnimationController.Start(windowCanvas, newOrigin, newZoom, navigationTimeOverride.value_or(editorStyle.NavigationDurationTime));
}

void CCurveEditorNavigationComponent::AdjustZoomLevelToZoom(const ax::pointf& zoom)
{
    const auto& editorStyle = GetEditorView().GetEditorStyle();

    const auto adjustZoomLevel = [&editorStyle](float originalZoom, float& destLevel)
    {
        if (originalZoom == 1.0f)
        {
            destLevel = 0.0f;
            return;
        }

        auto level = 1;
        auto zoom = 1.0f;

        if (originalZoom > 1.0f)
        {
            while (zoom < originalZoom && ++level)
                zoom *= editorStyle.ZoomInMultipler;

            destLevel = (float)(level - 1);
        }
        else
        {
            while (zoom > originalZoom && ++level)
                zoom *= editorStyle.ZoomOutMultipler;

            destLevel = -(float)(level - 1);
        }

    };

    adjustZoomLevel(zoom.x, m_ZoomLevel.x);
    adjustZoomLevel(zoom.y, m_ZoomLevel.y);
}

ICurveEditorNavigationComponentUniquePtr ICurveEditorNavigationComponent::Create(ICurveEditorView& editorView)
{
    return std::make_unique<CCurveEditorNavigationComponent>(editorView);
}