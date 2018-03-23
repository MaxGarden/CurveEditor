#include "pch.h"
#include "CurveEditorViewController.h"
#include "CurveEditorController.h"
#include "CurveEditorProtocolBase.h"

class CurveEditorViewControllerProtocol final : public CCurveEditorProtocolBase
{
public:
    CurveEditorViewControllerProtocol(CCurveEditorViewController& viewController);
    virtual ~CurveEditorViewControllerProtocol() override final = default;

    virtual void OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController) override;
    virtual void OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController) override;

private:
    CCurveEditorViewController& m_Controller;
};

CurveEditorViewControllerProtocol::CurveEditorViewControllerProtocol(CCurveEditorViewController& viewController) :
    m_Controller(viewController)
{
}

void CurveEditorViewControllerProtocol::OnSplineCreated(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    const auto result = m_Controller.CreateSplineView(splineController);
    EDITOR_ASSERT(result);
}

void CurveEditorViewControllerProtocol::OnSplineDestroyed(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    const auto result = m_Controller.DestroySplineView(splineController);
    EDITOR_ASSERT(result);
}

CCurveEditorViewController::CCurveEditorViewController(CCurveEditorController& editorController) :
    m_EditorController(editorController)
{
    const auto result = m_EditorController.RegisterProtocol(std::make_unique<CurveEditorViewControllerProtocol>(*this));
    EDITOR_ASSERT(result);
}

CCurveEditorViewController::~CCurveEditorViewController()
{
    const auto result = m_EditorController.UnregisterProtocol(m_ProtocolHandle);
    EDITOR_ASSERT(result);
}

void CCurveEditorViewController::OnDataModelChanged()
{
    //TODO
}

CCurveEditorController& CCurveEditorViewController::GetEditorController() noexcept
{
    return m_EditorController;
}

const CCurveEditorController& CCurveEditorViewController::GetEditorController() const noexcept
{
    return m_EditorController;
}

bool CCurveEditorViewController::CreateSplineView(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    return false;
}

bool CCurveEditorViewController::DestroySplineView(const ICurveEditorSplineControllerSharedPtr& splineController)
{
    return false;
}