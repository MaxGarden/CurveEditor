#include "pch.h"
#include "SelectionComponent.h"
#include "Components/SplinesComponent.h"
#include "CurveEditorViewVisibleComponentBase.h"
#include "CurveEditorSelectionView.h"
#include "CurveEditorSelectionController.h"
#include "SplineView.h"

class CCurveEditorSelectionViewComponent final : public CCurveEditorViewVisibleComponentBase<ICurveEditorSelectionViewComponent>
{
public:
    CCurveEditorSelectionViewComponent(ICurveEditorView& editorView);
    virtual ~CCurveEditorSelectionViewComponent() override final = default;

    virtual bool Initialize() override final;

    virtual void OnFrame() override final;

    virtual void ClearSelection() override final;

    virtual bool SetSelectionMode(ECurveEditorSplineComponentType selectionMode) noexcept override final;
    virtual std::optional<ECurveEditorSplineComponentType> GetSelectionMode() const noexcept override final;

    virtual bool AddToSelection(const CurveEditorViewSelection& selection) override final;
    virtual bool RemoveFromSelection(const CurveEditorViewSelection& selection) override final;

    virtual bool CheckIfSelected(const ICurveEditorSplineComponentView& splineComponent) const noexcept override final;

    virtual void VisitSelection(const VisitorType<ICurveEditorSplineComponentView>& visitor) const override final;

protected:
    virtual void OnControllerChanged() override final;

private:
    ICurveEditorSelectionController* GetSelectionController() const noexcept;

    CurveEditorDataModelSingleSelection TransformSingleSelection(const ICurveEditorSplineComponentView& splineComponentView) const noexcept;
    CurveEditorDataModelSelection TransformSelectiom(const CurveEditorViewSelection& selection) const noexcept;

private:
    ICurveEditorSelectionViewUniquePtr m_SelectionView;
    ICurveEditorSplinesViewComponentWeakPtr m_SplinesViewComponent;
};

CCurveEditorSelectionViewComponent::CCurveEditorSelectionViewComponent(ICurveEditorView& editorView) :
    CCurveEditorViewVisibleComponentBase(editorView)
{
}

bool CCurveEditorSelectionViewComponent::Initialize()
{
    m_SelectionView = ICurveEditorSelectionView::Create();
    EDITOR_ASSERT(m_SelectionView);
    if (!m_SelectionView)
        return false;

    m_SplinesViewComponent = GetViewComponent<ICurveEditorSplinesViewComponent>(GetEditorView());
    if (m_SplinesViewComponent.expired())
        return false;

    const auto result = m_SelectionView->SetSplineViewGetter([this](const auto& splineID) -> ICurveEditorSplineView*
    {
        if (const auto splineViewComponent = m_SplinesViewComponent.lock())
            return splineViewComponent->GetSplineView(splineID);

        return nullptr;
    });

    EDITOR_ASSERT(result);
    if (!result)
        return false;

    return !m_SplinesViewComponent.expired();
}

void CCurveEditorSelectionViewComponent::OnFrame()
{
    EDITOR_ASSERT(m_SelectionView);
    if (m_SelectionView)
        m_SelectionView->OnFrame();
}

bool CCurveEditorSelectionViewComponent::SetSelectionMode(ECurveEditorSplineComponentType selectionMode) noexcept
{
    const auto selectionController = GetSelectionController();
    EDITOR_ASSERT(selectionController);
    if (!selectionController)
        return false;

    return selectionController->SetSelectionMode(selectionMode);
}

std::optional<ECurveEditorSplineComponentType> CCurveEditorSelectionViewComponent::GetSelectionMode() const noexcept
{
    const auto selectionController = GetSelectionController();
    EDITOR_ASSERT(selectionController);
    if (!selectionController)
        return std::nullopt;

    return selectionController->GetSelectionMode();
}

void CCurveEditorSelectionViewComponent::ClearSelection()
{
    const auto selectionController = GetSelectionController();
    EDITOR_ASSERT(selectionController);
    if (!selectionController)
        return;

    const auto result = selectionController->ClearSelection();
    EDITOR_ASSERT(result);
}

bool CCurveEditorSelectionViewComponent::AddToSelection(const CurveEditorViewSelection& selection)
{
    const auto selectionController = GetSelectionController();
    EDITOR_ASSERT(selectionController);
    if (!selectionController)
        return false;

    const auto result = selectionController->AddToSelection(TransformSelectiom(selection));
    EDITOR_ASSERT(result);

    return result;
}

bool CCurveEditorSelectionViewComponent::RemoveFromSelection(const CurveEditorViewSelection& selection)
{
    const auto selectionController = GetSelectionController();
    EDITOR_ASSERT(selectionController);
    if (!selectionController)
        return false;

    const auto result = selectionController->RemoveFromSelection(TransformSelectiom(selection));
    EDITOR_ASSERT(result);

    return result;
}

bool CCurveEditorSelectionViewComponent::CheckIfSelected(const ICurveEditorSplineComponentView& splineComponent) const noexcept
{
    const auto selectionController = GetSelectionController();
    EDITOR_ASSERT(selectionController);
    if (!selectionController)
        return false;

    return selectionController->CheckIfSelected(TransformSingleSelection(splineComponent));
}

void CCurveEditorSelectionViewComponent::VisitSelection(const VisitorType<ICurveEditorSplineComponentView>& visitor) const
{
    if (!visitor)
        return;

    const auto selectionController = GetSelectionController();
    EDITOR_ASSERT(selectionController);
    if (!selectionController)
        return;

    const auto splinesViewComponent = m_SplinesViewComponent.lock();
    EDITOR_ASSERT(splinesViewComponent);
    if (!splinesViewComponent)
        return;

    selectionController->VisitSelection([&](const auto& splineComponentController)
    {
        EDITOR_ASSERT(splineComponentController);
        if (!splineComponentController)
            return;

        const auto splineView = splinesViewComponent->GetSplineView(splineComponentController->GetSplineID());
        EDITOR_ASSERT(splineView);
        if (!splineView)
            return;

        const auto splineComponentView = splineView->GetSplineComponent(*splineComponentController);
        EDITOR_ASSERT(splineComponentView);
        if (!splineComponentView)
            return;

        visitor(*splineComponentView);
    });
}

void CCurveEditorSelectionViewComponent::OnControllerChanged()
{
    if (!m_SelectionView)
        return;

    if (const auto& controller = GetController())
        m_SelectionView->SetController(controller->GetSelectionController());
    else
        m_SelectionView->SetController(nullptr);
}

ICurveEditorSelectionController* CCurveEditorSelectionViewComponent::GetSelectionController() const noexcept
{
    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return nullptr;

    return controller->GetSelectionController().get();
}

CurveEditorDataModelSingleSelection CCurveEditorSelectionViewComponent::TransformSingleSelection(const ICurveEditorSplineComponentView& splineComponentView) const noexcept
{
    const auto& componentIndex = splineComponentView.GetIndex();
    EDITOR_ASSERT(componentIndex);
    if (!componentIndex)
        return{};

    return std::make_pair<>(splineComponentView.GetSplineID(), *componentIndex);
}

CurveEditorDataModelSelection CCurveEditorSelectionViewComponent::TransformSelectiom(const CurveEditorViewSelection& selection) const noexcept
{
    CurveEditorDataModelSelection result;

    VisitPointersContainer(selection, [this, &result](const auto& splineComponentView)
    {
        result.emplace(TransformSingleSelection(splineComponentView));
    });

    EDITOR_ASSERT(result.size() == selection.size());
    return result;
}

ICurveEditorSelectionViewComponentUniquePtr ICurveEditorSelectionViewComponent::Create(ICurveEditorView& editorView)
{
    return std::make_unique<CCurveEditorSelectionViewComponent>(editorView);
}