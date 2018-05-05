#include "pch.h"
#include "SelectionComponent.h"
#include "CurveEditorListenerBase.h"
#include "Components/SplinesComponent.h"
#include "CurveEditorViewVisibleComponentBase.h"

class CCurveEditorSelectionViewComponent final : public CCurveEditorViewVisibleComponentBase<ICurveEditorSelectionViewComponent>
{
public:
    CCurveEditorSelectionViewComponent(ICurveEditorView& editorView);
    virtual ~CCurveEditorSelectionViewComponent() override final = default;

    virtual bool Initialize() override final;

    virtual void OnFrame() override final;

    virtual void ClearSelection() override final;

    virtual bool AddToSelection(const ICurveEditorSplineComponentView& splineComponent) override final;
    virtual bool AddToSelection(const std::set<const ICurveEditorSplineComponentView*>& splineComponents) override final;

    virtual bool RemoveFromSelection(const ICurveEditorSplineComponentView& splineComponent) override final;
    virtual bool RemoveFromSelection(const std::set<const ICurveEditorSplineComponentView*>& splineComponents) override final;

    virtual bool CheckIfSelected(const ICurveEditorSplineComponentView& splineComponent) const noexcept override final;

    void OnSelectionChanged();

protected:
    virtual IEditorListenerUniquePtr CreateListener() override final;

private:
    ICurveEditorSplinesViewComponentWeakPtr m_SplineViewComponent;
    std::map<const ICurveEditorSplineComponentView*, IEditorRenderableUniquePtr> m_SelectionBorders;
};

class CCurveEditorSelectionViewComponentListener final : public CCurveEditorControllerListenerBase
{
public:
    CCurveEditorSelectionViewComponentListener(CCurveEditorSelectionViewComponent& selectionViewComponent);
    virtual ~CCurveEditorSelectionViewComponentListener() override final = default;

    virtual void OnSelectionChanged() override final;

private:
    CCurveEditorSelectionViewComponent& m_SelectionViewComponent;
};


CCurveEditorSelectionViewComponentListener::CCurveEditorSelectionViewComponentListener(CCurveEditorSelectionViewComponent& selectionViewComponent) :
    m_SelectionViewComponent(selectionViewComponent)
{
}

void CCurveEditorSelectionViewComponentListener::OnSelectionChanged()
{
    m_SelectionViewComponent.OnSelectionChanged();
}

CCurveEditorSelectionViewComponent::CCurveEditorSelectionViewComponent(ICurveEditorView& editorView) :
    CCurveEditorViewVisibleComponentBase(editorView)
{
}

bool CCurveEditorSelectionViewComponent::Initialize()
{
    m_SplineViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(GetEditorView());

    return !m_SplineViewComponent.expired();
}

void CCurveEditorSelectionViewComponent::OnFrame()
{
    VisitPointersMap(m_SelectionBorders, [](auto& selectionBorder)
    {
        selectionBorder.OnFrame();
    });
}

void CCurveEditorSelectionViewComponent::ClearSelection()
{
    m_SelectionBorders.clear();
}

bool CCurveEditorSelectionViewComponent::AddToSelection(const ICurveEditorSplineComponentView& splineComponent)
{
    return AddToSelection(std::set<const ICurveEditorSplineComponentView*>{ &splineComponent });
}

bool CCurveEditorSelectionViewComponent::AddToSelection(const std::set<const ICurveEditorSplineComponentView*>& splineComponents)
{
    for (const auto splineComponent : splineComponents)
    {
        EDITOR_ASSERT(splineComponent);
        if (!splineComponent || m_SelectionBorders.find(splineComponent) != m_SelectionBorders.cend())
            continue;

        auto selectionBorder = splineComponent->CreateBorderRenderable(CurveEditorStyleColor_SelectionBorder, CurveEditorStyleFloat_SelectionBorderThickness);
        EDITOR_ASSERT(selectionBorder);
        if (!selectionBorder)
            continue;

        m_SelectionBorders.emplace(splineComponent, std::move(selectionBorder));
    }

    return true;
}

bool CCurveEditorSelectionViewComponent::RemoveFromSelection(const ICurveEditorSplineComponentView& splineComponent)
{
    const auto iterator = m_SelectionBorders.find(&splineComponent);
    if (iterator == m_SelectionBorders.cend())
        return false;

    m_SelectionBorders.erase(iterator);
    return true;
}

bool CCurveEditorSelectionViewComponent::RemoveFromSelection(const std::set<const ICurveEditorSplineComponentView*>& splineComponents)
{
    for (const auto splineComponent : splineComponents)
    {
        EDITOR_ASSERT(splineComponent);
        if (!splineComponent)
            continue;

        const auto iterator = m_SelectionBorders.find(splineComponent);
        EDITOR_ASSERT(iterator != m_SelectionBorders.cend());
        if (iterator == m_SelectionBorders.cend())
            continue;

        m_SelectionBorders.erase(iterator);
    }

    return true;
}

bool CCurveEditorSelectionViewComponent::CheckIfSelected(const ICurveEditorSplineComponentView& splineComponent) const noexcept
{
    return m_SelectionBorders.find(&splineComponent) != m_SelectionBorders.cend();
}

void CCurveEditorSelectionViewComponent::OnSelectionChanged()
{
}

IEditorListenerUniquePtr CCurveEditorSelectionViewComponent::CreateListener()
{
    return std::make_unique<CCurveEditorSelectionViewComponentListener>(*this);
}

ICurveEditorSelectionViewComponentUniquePtr ICurveEditorSelectionViewComponent::Create(ICurveEditorView& editorView)
{
    return std::make_unique<CCurveEditorSelectionViewComponent>(editorView);
}