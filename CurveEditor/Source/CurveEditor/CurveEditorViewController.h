#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_CONTROLLER_H__)

#include "Style.h"
#include "CurveEditorViewDataModel.h"
#include "EditorControllerBase.h"

class ICurveEditorViewProtocol : public IEditorProtocol
{
public:
    virtual ~ICurveEditorViewProtocol() = default;
};

class CCurveEditorViewController final : public CEditorControllerBase<IEditorController, ICurveEditorViewDataModel, ICurveEditorViewProtocol>
{
public:
    CCurveEditorViewController(CCurveEditorController& editorController);
    virtual ~CCurveEditorViewController() override final;

    CCurveEditorController& GetEditorController() noexcept;
    const CCurveEditorController& GetEditorController() const noexcept;

    bool CreateSplineView(const ICurveEditorSplineControllerSharedPtr& splineController);
    bool DestroySplineView(const ICurveEditorSplineControllerSharedPtr& splineController);

private:
    virtual void OnDataModelChanged() override final;

private:
    CCurveEditorController& m_EditorController;
    EditorProtocolHandle m_ProtocolHandle;
};

#endif //__CURVE_EDITOR_VIEW_CONTROLLER_H__