#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_H__)

#include "Canvas.h"
#include "CurveEditorController.h"
#include "EditorViewBase.h"

class ICurveEditorView : public IEditorView
{
public:
    virtual ~ICurveEditorView() override = default;

    virtual CEditorCanvas& GetCanvas() noexcept = 0;
    virtual const CEditorCanvas& GetCanvas() const noexcept = 0;

    virtual bool AddViewComponent(IEditorViewUniquePtr&& view) = 0;

    virtual const ICurveEditorControllerSharedPtr& GetController() const noexcept = 0;

    static ICurveEditorViewUniquePtr Create(ICurveEditorSplineViewFactory& splinceViewFactory);
};

#endif //__CURVE_EDITOR_VIEW_H__