#include "pch.h"
#include "SplinesComponent.h"
#include "CurveEditorViewComponentBase.h"
#include "SplineView.h"
#include "SplineController.h"
#include "CurveEditorListenerBase.h"

class CCurveEditorSplinesViewComponent final : public CCurveEditorViewComponentBase<ICurveEditorSplinesViewComponent>
{
public:
    CCurveEditorSplinesViewComponent(ICurveEditorView& editorView, ICurveEditorSplineViewFactory& splineViewFactory);
    virtual ~CCurveEditorSplinesViewComponent() override final = default;

    virtual void OnFrame() override final;

    virtual ICurveEditorSplineView* GetSplineView(const SplineID& id) const noexcept override final;

    virtual bool RemoveSpline(const SplineID& id) override final;

    virtual ICurveEditorSplineComponentView* GetSplineComponentAt(const ax::pointf& position, std::optional<ECurveEditorSplineComponentType> componentType = std::nullopt, float extraThickness = 0.0f) const noexcept override final;
    virtual void VisitSplineComponentsInRect(const VisitorType<ICurveEditorSplineComponentView>& visitor, const ax::rectf& rect, std::optional<ECurveEditorSplineComponentType> componentType = std::nullopt, bool allowIntersect = true) const noexcept override final;

    bool CreateSplineView(const ICurveEditorSplineControllerSharedPtr& splineController);
    bool DestroySplineView(const ICurveEditorSplineControllerConstSharedPtr& splineController);

protected:
    virtual void OnControllerChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

private:
    void VisitSplineViews(const InterruptibleVisitorType<ICurveEditorSplineView>& visitor, bool reverse = false) const noexcept;
    void RecreateSplineViews();

private:
    std::map<ICurveEditorSplineControllerConstSharedPtr, ICurveEditorSplineViewUniquePtr> m_SplineViews;
    ICurveEditorSplineViewFactory& m_SplineViewFactory;

    EditorListenerHandle m_ListenerHandle;
};

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
        return true;
    });
}

ICurveEditorSplineView* CCurveEditorSplinesViewComponent::GetSplineView(const SplineID& id) const noexcept
{
    const auto iterator = std::find_if(m_SplineViews.cbegin(), m_SplineViews.cend(), [&id](const auto& pair)
    {
        const auto& splineController = pair.first;
        return splineController && splineController->GetID() == id;
    });

    if (iterator == m_SplineViews.cend())
        return nullptr;

    return iterator->second.get();
}

bool CCurveEditorSplinesViewComponent::RemoveSpline(const SplineID& id)
{
    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return false;

    return controller->RemoveSpline(id);
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

IEditorListenerUniquePtr CCurveEditorSplinesViewComponent::CreateListener()
{
    return std::make_unique<CCurveEditorViewListener>(*this);
}

void CCurveEditorSplinesViewComponent::VisitSplineViews(const InterruptibleVisitorType<ICurveEditorSplineView>& visitor, bool reverse) const noexcept
{
    VisitPointersMapInterruptible(m_SplineViews, visitor, reverse);
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

ICurveEditorSplineComponentView* CCurveEditorSplinesViewComponent::GetSplineComponentAt(const ax::pointf& position, std::optional<ECurveEditorSplineComponentType> componentType /* = std::nullopt */, float extraThickness /* = 0.0f */) const noexcept
{
    ICurveEditorSplineComponentView* result = nullptr;

    const auto componentVisitor = [&](auto& splineComponent)
    {
        if (splineComponent.IsColliding(position, extraThickness))
        {
            result = &splineComponent;
            return false;
        }

        return true;
    };

    if (componentType)
    {
        VisitSplineViews([&](const auto& splineView)
        {
            splineView.VisitSplineComponents(*componentType, componentVisitor, true);
            return result == nullptr;
        }, true);
    }
    else
    {
        const auto visitComponentType = [&](const auto& splineView, const auto componentType)
        {
            if (!result)
                splineView.VisitSplineComponents(componentType, componentVisitor, true);
        };

        VisitSplineViews([&](const auto& splineView)
        {
            visitComponentType(splineView, ECurveEditorSplineComponentType::Tangent);
            visitComponentType(splineView, ECurveEditorSplineComponentType::Knot);
            visitComponentType(splineView, ECurveEditorSplineComponentType::Curve);

            return result == nullptr;
        }, true);
    }

    return result;
}

void CCurveEditorSplinesViewComponent::VisitSplineComponentsInRect(const VisitorType<ICurveEditorSplineComponentView>& visitor, const ax::rectf& rect, std::optional<ECurveEditorSplineComponentType> componentType /* = std::nullopt */, bool allowIntersect /* = true */) const noexcept
{
    if (!visitor)
        return;

    const auto componentVisitor = [&](auto& splineComponent)
    {
        if (splineComponent.IsColliding(rect, allowIntersect))
            visitor(splineComponent);

        return true;
    };

    if (componentType)
    {
        VisitSplineViews([&](const auto& splineView)
        {
            splineView.VisitSplineComponents(*componentType, componentVisitor, true);
            return true;
        });
    }
    else
    {
        VisitSplineViews([&](const auto& splineView)
        {
            splineView.VisitSplineComponents(ECurveEditorSplineComponentType::Tangent, componentVisitor, true);
            splineView.VisitSplineComponents(ECurveEditorSplineComponentType::Knot, componentVisitor, true);
            splineView.VisitSplineComponents(ECurveEditorSplineComponentType::Curve, componentVisitor, true);
            return true;
        });
    }
}

ICurveEditorSplinesViewComponentUniquePtr ICurveEditorSplinesViewComponent::Create(ICurveEditorView& editorView, ICurveEditorSplineViewFactory& splineViewFactory)
{
    return std::make_unique<CCurveEditorSplinesViewComponent>(editorView, splineViewFactory);
}