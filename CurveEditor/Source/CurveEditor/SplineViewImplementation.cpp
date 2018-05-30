#include "pch.h"
#include "SplineViewImplementation.h"
#include "CurveEditorSplineListenerBase.h"
#include "KnotController.h"
#include "TangentController.h"
#include "CurveController.h"

class CCurveEditorSplineView final : public CEditorViewBase<ICurveEditorSplineView, ICurveEditorSplineController>
{
public:
    CCurveEditorSplineView(ICurveEditorView& editorView);
    virtual ~CCurveEditorSplineView() override final = default;

    virtual void OnFrame() override final;

    virtual bool BeginEditing() override final;
    virtual bool EndEditing() override final;

    virtual bool SaveState() override final;
    virtual bool RestoreState() override final;
    virtual bool ResetSavedState() noexcept override final;

    virtual ICurveEditorSplineComponentViewSharedPtr GetSplineComponent(const ICurveEditorSplineComponentController& splineComponentController) const noexcept override final;

    virtual void VisitSplineComponents(ECurveEditorSplineComponentType componentType, const InterruptibleVisitorType<ICurveEditorSplineComponentView>& visitor, bool reverse = false) const noexcept override final;

    bool CreateKnotView(const ICurveEditorKnotControllerSharedPtr& knotController);
    bool DestroyKnotView(const ICurveEditorKnotControllerSharedPtr& knotController);

    bool CreateTangentView(const ICurveEditorTangentControllerSharedPtr& tangentController);
    bool DestroyTangentView(const ICurveEditorTangentControllerSharedPtr& tangentController);

    bool CreateCurveView(const ICurveEditorCurveControllerSharedPtr& curveController);
    bool DestroyCurveView(const ICurveEditorCurveControllerSharedPtr& curveController);

protected:
    virtual void OnControllerChanged() override final;
    virtual IEditorListenerUniquePtr CreateListener() override final;

private:
    void VisitCurvesViews(const InterruptibleVisitorType<ICurveEditorCurveView>& visitor, bool reverse = false) const noexcept;
    void VisitKnotsViews(const InterruptibleVisitorType<ICurveEditorKnotView>& visitor, bool reverse = false) const noexcept;
    void VisitTangentsViews(const InterruptibleVisitorType<ICurveEditorTangentView>& visitor, bool reverse = false) const noexcept;

    void CreateKnotViews(ICurveEditorSplineController& controller);
    void CreateTangentViews(ICurveEditorSplineController& controller);
    void CreateCurveViews(ICurveEditorSplineController& controller);

private:
    ICurveEditorView & m_EditorView;
    std::map<ICurveEditorCurveControllerSharedPtr, ICurveEditorCurveViewSharedPtr> m_CurvesViews;
    std::map<ICurveEditorKnotControllerSharedPtr, ICurveEditorKnotViewSharedPtr> m_KnotsViews;
    std::map<ICurveEditorTangentControllerSharedPtr, ICurveEditorTangentViewSharedPtr> m_TangentsViews;
};

class CCurveEditorSplineViewListener final : public CCurveEditorSplineControllerListenerBase
{
public:
    CCurveEditorSplineViewListener(CCurveEditorSplineView& splineView);
    virtual ~CCurveEditorSplineViewListener() override final = default;

    virtual void OnKnotCreated(const ICurveEditorKnotControllerSharedPtr& knotController) override final;
    virtual void OnKnotDestroyed(const ICurveEditorKnotControllerSharedPtr& knotController) override final;

    virtual void OnTangentCreated(const ICurveEditorTangentControllerSharedPtr& tangentController) override final;
    virtual void OnTangentDestroyed(const ICurveEditorTangentControllerSharedPtr& tangentController) override final;

    virtual void OnCurveCreated(const ICurveEditorCurveControllerSharedPtr& curveController) override final;
    virtual void OnCurveDestroyed(const ICurveEditorCurveControllerSharedPtr& curveController) override final;

private:
    CCurveEditorSplineView& m_SplineView;
};

CCurveEditorSplineViewListener::CCurveEditorSplineViewListener(CCurveEditorSplineView& splineView) :
    m_SplineView(splineView)
{
}

void CCurveEditorSplineViewListener::OnKnotCreated(const ICurveEditorKnotControllerSharedPtr& knotController)
{
    const auto result = m_SplineView.CreateKnotView(knotController);
    EDITOR_ASSERT(result);
}

void CCurveEditorSplineViewListener::OnKnotDestroyed(const ICurveEditorKnotControllerSharedPtr& knotController)
{
    const auto result = m_SplineView.DestroyKnotView(knotController);
    EDITOR_ASSERT(result);
}

void CCurveEditorSplineViewListener::OnTangentCreated(const ICurveEditorTangentControllerSharedPtr& tangentController)
{
    const auto result = m_SplineView.CreateTangentView(tangentController);
    EDITOR_ASSERT(result);
}

void CCurveEditorSplineViewListener::OnTangentDestroyed(const ICurveEditorTangentControllerSharedPtr& tangentController)
{
    const auto result = m_SplineView.DestroyTangentView(tangentController);
    EDITOR_ASSERT(result);
}

void CCurveEditorSplineViewListener::OnCurveCreated(const ICurveEditorCurveControllerSharedPtr& curveController)
{
    const auto result = m_SplineView.CreateCurveView(curveController);
    EDITOR_ASSERT(result);
}

void CCurveEditorSplineViewListener::OnCurveDestroyed(const ICurveEditorCurveControllerSharedPtr& curveController)
{
    const auto result = m_SplineView.DestroyCurveView(curveController);
    EDITOR_ASSERT(result);
}

CCurveEditorSplineView::CCurveEditorSplineView(ICurveEditorView& editorView) :
    m_EditorView(editorView)
{
}

void CCurveEditorSplineView::OnFrame()
{
    static const auto onFrameVisitor = [](auto& view)
    {
        view.OnFrame();
        return true;
    };

    VisitCurvesViews(onFrameVisitor);
    VisitTangentsViews(onFrameVisitor);
    VisitKnotsViews(onFrameVisitor);
}

bool CCurveEditorSplineView::BeginEditing()
{
    if (const auto& controller = GetController())
        return controller->BeginEditing();

    EDITOR_ASSERT(false);
    return false;
}

bool CCurveEditorSplineView::EndEditing()
{
    if (const auto& controller = GetController())
        return controller->EndEditing();

    EDITOR_ASSERT(false);
    return false;
}

bool CCurveEditorSplineView::SaveState()
{
    if (const auto& controller = GetController())
        return controller->SaveState();

    EDITOR_ASSERT(false);
    return false;
}

bool CCurveEditorSplineView::RestoreState()
{
    if (const auto& controller = GetController())
        return controller->RestoreState();

    EDITOR_ASSERT(false);
    return false;
}

bool CCurveEditorSplineView::ResetSavedState() noexcept
{
    const auto& controller = GetController();
    EDITOR_ASSERT(controller);
    if (!controller)
        return false;

    controller->ResetSavedState();
    return true;
}

template<typename ContainerType>
static auto FindSplineComponent(const ContainerType& container, const ICurveEditorSplineComponentController& splineComponentController) noexcept -> ICurveEditorSplineComponentViewSharedPtr
{
    const auto iterator = std::find_if(container.cbegin(), container.cend(), [splineComponentController = &splineComponentController](const auto& pair)
    {
        return pair.first.get() == splineComponentController;
    });

    if (iterator == container.cend())
        return nullptr;

    return iterator->second;
}

ICurveEditorSplineComponentViewSharedPtr CCurveEditorSplineView::GetSplineComponent(const ICurveEditorSplineComponentController& splineComponentController) const noexcept
{
    switch (splineComponentController.GetType())
    {
    case ECurveEditorSplineComponentType::Curve:
        return FindSplineComponent(m_CurvesViews, splineComponentController);
    case ECurveEditorSplineComponentType::Knot:
        return FindSplineComponent(m_KnotsViews, splineComponentController);
    case ECurveEditorSplineComponentType::Tangent:
        return FindSplineComponent(m_TangentsViews, splineComponentController);
    default:
        EDITOR_ASSERT(false);
        return nullptr;
    }
}

void CCurveEditorSplineView::OnControllerChanged()
{
    m_CurvesViews.clear();
    m_KnotsViews.clear();
    m_TangentsViews.clear();

    const auto& controller = GetController();
    if (!controller)
        return;

    CreateTangentViews(*controller);
    CreateKnotViews(*controller);
    CreateCurveViews(*controller);
}

IEditorListenerUniquePtr CCurveEditorSplineView::CreateListener()
{
    return std::make_unique<CCurveEditorSplineViewListener>(*this);
}

void CCurveEditorSplineView::VisitCurvesViews(const InterruptibleVisitorType<ICurveEditorCurveView>& visitor, bool reverse /*= false*/) const noexcept
{
    VisitPointersMap(m_CurvesViews, visitor, reverse);
}

void CCurveEditorSplineView::VisitKnotsViews(const InterruptibleVisitorType<ICurveEditorKnotView>& visitor, bool reverse /*= false*/) const noexcept
{
    VisitPointersMap(m_KnotsViews, visitor, reverse);
}

void CCurveEditorSplineView::VisitTangentsViews(const InterruptibleVisitorType<ICurveEditorTangentView>& visitor, bool reverse /*= false*/) const noexcept
{
    VisitPointersMap(m_TangentsViews, visitor, reverse);
}

void CCurveEditorSplineView::VisitSplineComponents(ECurveEditorSplineComponentType componentType, const InterruptibleVisitorType<ICurveEditorSplineComponentView>& visitor, bool reverse /*= false*/) const noexcept
{
    switch (componentType)
    {
    case ECurveEditorSplineComponentType::Knot:
        VisitKnotsViews(visitor, reverse);
        break;
    case ECurveEditorSplineComponentType::Curve:
        VisitCurvesViews(visitor, reverse);
        break;
    case ECurveEditorSplineComponentType::Tangent:
        VisitTangentsViews(visitor, reverse);
        break;
    default:
        EDITOR_ASSERT(false && "Unsupported component type");
        break;
    }
}

template<typename ViewType, typename Container, typename ControllerType>
static auto CreateView(Container& container, ICurveEditorView& editorView, const ControllerType& controller)
{
    EDITOR_ASSERT(controller);

    auto view = ViewType::Create(editorView);

    auto isValid = true;
    isValid &= view->SetController(controller);
    EDITOR_ASSERT(isValid);

    if (!isValid)
        return false;

    const auto result = container.try_emplace(controller, std::move(view));
    return result.second;
}

bool CCurveEditorSplineView::CreateKnotView(const ICurveEditorKnotControllerSharedPtr& knotController)
{
    return CreateView<ICurveEditorKnotView>(m_KnotsViews, m_EditorView, knotController);
}

bool CCurveEditorSplineView::DestroyKnotView(const ICurveEditorKnotControllerSharedPtr& knotController)
{
    return RemoveFromContainer(m_KnotsViews, knotController);
}

bool CCurveEditorSplineView::CreateTangentView(const ICurveEditorTangentControllerSharedPtr& tangentController)
{
    return CreateView<ICurveEditorTangentView>(m_TangentsViews, m_EditorView, tangentController);
}

bool CCurveEditorSplineView::DestroyTangentView(const ICurveEditorTangentControllerSharedPtr& tangentController)
{
    return RemoveFromContainer(m_TangentsViews, tangentController);
}

bool CCurveEditorSplineView::CreateCurveView(const ICurveEditorCurveControllerSharedPtr& curveController)
{
    return CreateView<ICurveEditorCurveView>(m_CurvesViews, m_EditorView, curveController);
}

bool CCurveEditorSplineView::DestroyCurveView(const ICurveEditorCurveControllerSharedPtr& curveController)
{
    return RemoveFromContainer(m_CurvesViews, curveController);
}

void CCurveEditorSplineView::CreateKnotViews(ICurveEditorSplineController& controller)
{
    controller.VisitKnotsControllers([this](auto& knotController)
    {
        CreateKnotView(knotController);
    });
}

void CCurveEditorSplineView::CreateTangentViews(ICurveEditorSplineController& controller)
{
    controller.VisitTangentsControllers([this](auto& tangentController)
    {
        CreateTangentView(tangentController);
    });
}

void CCurveEditorSplineView::CreateCurveViews(ICurveEditorSplineController& controller)
{
    controller.VisitCurvesControllers([this](auto& curveController)
    {
        CreateCurveView(curveController);
    });
}

ICurveEditorSplineViewUniquePtr CCurveEditorSplineViewFactory::Create(ICurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr&)
{
    return std::make_unique<CCurveEditorSplineView>(editorView);
}