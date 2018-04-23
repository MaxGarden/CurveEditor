#include "pch.h"
#include "SplinesComponent.h"
#include "SplineView.h"
#include "SplineController.h"
#include "CurveEditorListenerBase.h"

class CCurveEditorViewListener final : public CCurveEditorControllerListenerBase
{
public:
    CCurveEditorViewListener(CCurveEditorSplinesViewComponent& splinesView);
    virtual ~CCurveEditorViewListener() override final = default;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) override final;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) override final;

private:
    CCurveEditorSplinesViewComponent& m_SplinesView;
};

CCurveEditorViewListener::CCurveEditorViewListener(CCurveEditorSplinesViewComponent& splinesView) :
    m_SplinesView(splinesView)
{
}

void CCurveEditorViewListener::OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    const auto result = m_SplinesView.CreateSplineView(splineController);
    EDITOR_ASSERT(result);
}

void CCurveEditorViewListener::OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    const auto result = m_SplinesView.DestroySplineView(splineController);
    EDITOR_ASSERT(result);
}

CCurveEditorSplinesViewComponent::CCurveEditorSplinesViewComponent(ICurveEditorView& editorView, ICurveEditorSplineViewFactory& splineViewFactory) :
    CCurveEditorViewComponentBase(editorView),
    m_SplineViewFactory(splineViewFactory)
{
}

void CCurveEditorSplinesViewComponent::OnFrame()
{
    VisitSplineViews([](auto& splineView)
    {
        splineView.OnFrame();
    });
}

bool CCurveEditorSplinesViewComponent::SetController(const IEditorControllerSharedPtr& controller) noexcept
{
    const auto previousController = GetController();

    if (!Super::SetController(controller))
        return false;

    if (previousController)
        previousController->UnregisterListener(m_ListenerHandle);

    if (const auto& controller = GetController())
        m_ListenerHandle = controller->RegisterListener(std::make_unique<CCurveEditorViewListener>(*this)).value_or(0);

    RecreateSplineViews();
    return true;
}

bool CCurveEditorSplinesViewComponent::CreateSplineView(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    if (m_SplineViews.find(splineController) != m_SplineViews.end())
    {
        EDITOR_ASSERT(false && "This controller already has a view");
        return false;
    }

    auto splineView = m_SplineViewFactory.Create(GetEditorView(), splineController);
    if (!splineView)
        return false;

    auto isValid = true;
    isValid &= splineView->SetController(splineController);

    if (!isValid)
        return false;

    m_SplineViews.emplace(splineController, std::move(splineView));
    return true;
}

bool CCurveEditorSplinesViewComponent::DestroySplineView(const ICurveEditorSplineControllerConstSharedPtr& splineController)
{
    return RemoveFromContainer(m_SplineViews, splineController);
}

void CCurveEditorSplinesViewComponent::OnControllerChanged()
{
    RecreateSplineViews();
}

void CCurveEditorSplinesViewComponent::VisitSplineViews(const VisitorType<ICurveEditorSplineView>& visitor) noexcept
{
    if (!visitor)
        return;

    for (const auto& splineViewPair : m_SplineViews)
    {
        if (const auto& splineView = splineViewPair.second)
            visitor(*splineView);
    }
}

void CCurveEditorSplinesViewComponent::RecreateSplineViews()
{
    std::vector<ICurveEditorSplineControllerConstSharedPtr> splineControllersToDestroy;
    std::transform(m_SplineViews.begin(), m_SplineViews.end(), std::back_inserter(splineControllersToDestroy),
        [](const auto& pair)
    {
        return pair.first;
    });

    for (const auto& splineController : splineControllersToDestroy)
        DestroySplineView(splineController);

    const auto& controller = GetController();
    if (!controller)
        return;

    controller->VisitSplineControllers([this](const auto& controller)
    {
        CreateSplineView(controller);
    });
}