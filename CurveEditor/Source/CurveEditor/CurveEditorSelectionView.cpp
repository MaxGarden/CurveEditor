#include "pch.h"
#include "CurveEditorSelectionView.h"
#include "CurveEditorSelectionController.h"
#include "EditorViewBase.h"
#include "SplineController.h"
#include "CurveEditorSelectionListenerBase.h"
#include "Components/SplinesComponent.h"
#include "SplineView.h"

class CCurveEditorSelectionView final : public CEditorViewBase<ICurveEditorSelectionView, ICurveEditorSelectionController>
{
public:
    CCurveEditorSelectionView() = default;
    virtual ~CCurveEditorSelectionView() override final = default;

    virtual void OnFrame() override final;

    virtual bool SetSplineViewGetter(SplineViewGetter splineViewGetter) override final;

    bool AddToSelection(const ICurveEditorSplineComponentController& splineComponentController);
    bool AddToSelection(const CurveEditorControllerSelection& selection);
    bool RemoveFromSelection(const CurveEditorControllerSelection& selection, bool clear);

protected:
    virtual void OnControllerChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

private:
    SplineViewGetter m_SplineViewGetter;
    std::map<const ICurveEditorSplineComponentController*, IEditorRenderableUniquePtr> m_SelectionBorders;
};

class CCurveEditorSelectionViewListener final : public CCurveEditorSelectionControllerListenerBase
{
public:
    CCurveEditorSelectionViewListener(CCurveEditorSelectionView& selectionView);
    virtual ~CCurveEditorSelectionViewListener() override final = default;

    virtual void OnAddedToSelection(const CurveEditorControllerSelection& selection) override final;
    virtual void OnRemovedFromSelection(const CurveEditorControllerSelection& selection, bool clear) override final;

private:
    CCurveEditorSelectionView& m_SelectionView;
};

CCurveEditorSelectionViewListener::CCurveEditorSelectionViewListener(CCurveEditorSelectionView& selectionView) :
    m_SelectionView(selectionView)
{
}

void CCurveEditorSelectionViewListener::OnAddedToSelection(const CurveEditorControllerSelection& selection)
{
    const auto result = m_SelectionView.AddToSelection(selection);
    EDITOR_ASSERT(result);
}

void CCurveEditorSelectionViewListener::OnRemovedFromSelection(const CurveEditorControllerSelection& selection, bool clear)
{
    const auto result = m_SelectionView.RemoveFromSelection(selection, clear);
    EDITOR_ASSERT(result);
}

void CCurveEditorSelectionView::OnFrame()
{
    VisitPointersMap(m_SelectionBorders, [](auto& selectionBorder)
    {
        selectionBorder.OnFrame();
    });
}

bool CCurveEditorSelectionView::SetSplineViewGetter(SplineViewGetter splineViewGetter)
{
    if (!splineViewGetter)
        return false;

    m_SplineViewGetter = std::move(splineViewGetter);
    return true;
}

bool CCurveEditorSelectionView::AddToSelection(const ICurveEditorSplineComponentController& splineComponentController)
{
    EDITOR_ASSERT(m_SplineViewGetter);
    if (!m_SplineViewGetter)
        return false;

    const auto splineView = m_SplineViewGetter(splineComponentController.GetSplineID());
    if (!splineView)
        return false;

    const auto& splineComponent = splineView->GetSplineComponent(splineComponentController);
    if (!splineComponent)
        return false;

    auto selectionBorder = splineComponent->CreateBorderRenderable(CurveEditorStyleColor_SelectionBorder, CurveEditorStyleFloat_SelectionBorderThickness);
    EDITOR_ASSERT(selectionBorder);
    if (!selectionBorder)
        return false;

    m_SelectionBorders.try_emplace(&splineComponentController, std::move(selectionBorder));
    return true;
}

bool CCurveEditorSelectionView::AddToSelection(const CurveEditorControllerSelection& selection)
{
    auto result = true;

    VisitPointersContainer(selection, [this, &result](const auto& splineComponentController)
    {
        result &= AddToSelection(splineComponentController);
        EDITOR_ASSERT(result);
    });

    return result;
}

bool CCurveEditorSelectionView::RemoveFromSelection(const CurveEditorControllerSelection& selection, bool clear)
{
    if (clear)
    {
        m_SelectionBorders.clear();
        return true;
    }

    auto result = true;

    VisitPointersContainer(selection, [this, &result](const auto& splineComponentController)
    {
        result &= m_SelectionBorders.erase(&splineComponentController) == 1;
        EDITOR_ASSERT(result);
    });

    return result;
}

void CCurveEditorSelectionView::OnControllerChanged()
{
    m_SelectionBorders.clear();

    const auto& controller = GetController();
    if (!controller)
        return;

    controller->VisitSelection([this](const auto& splineComponentController)
    {
        EDITOR_ASSERT(splineComponentController);
        if (!splineComponentController)
            return;

        const auto result = AddToSelection(*splineComponentController);
        EDITOR_ASSERT(result);
    });
}

IEditorListenerUniquePtr CCurveEditorSelectionView::CreateListener()
{
    return std::make_unique<CCurveEditorSelectionViewListener>(*this);
}

ICurveEditorSelectionViewUniquePtr ICurveEditorSelectionView::Create()
{
    return std::make_unique<CCurveEditorSelectionView>();
}