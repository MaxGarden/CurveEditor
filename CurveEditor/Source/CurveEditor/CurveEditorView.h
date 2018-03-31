#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_H__)

#include "Canvas.h"
#include "CurveEditorController.h"
#include "EditorViewBase.h"

class CCurveEditorView final : public CEditorViewBase<IEditorView, CCurveEditorController>
{
public:
    CCurveEditorView(ICurveEditorSplineViewFactory& splineViewFactory);
    virtual ~CCurveEditorView();

    virtual void OnFrame() override final;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept override;

    bool CreateSplineView(const ICurveEditorSplineControllerSharedPtr& splineController);
    bool DestroySplineView(const ICurveEditorSplineControllerConstSharedPtr& splineController);

    CEditorCanvas& GetCanvas() noexcept;
    const CEditorCanvas& GetCanvas() const noexcept;

    bool AddView(IEditorViewUniquePtr&& view);

protected:
    virtual void OnControllerChanged() noexcept override final;

private:
    void VisitViews(const std::function<void(IEditorView&)>& visitor) noexcept;
    void VisitSplineViews(const std::function<void(ICurveEditorSplineView&)>& visitor) noexcept;

    void RecreateSplineViews();

    void OnFrameBegin();
    void OnFrameEnd();

    void SetWindowCanvas();
    void ApplyCanvas();

private:
    CEditorCanvas m_Canvas = CEditorCanvas({ 100.0f, 100.0f });

    std::vector<IEditorViewUniquePtr> m_Views;
    std::map<ICurveEditorSplineControllerConstSharedPtr, ICurveEditorSplineViewUniquePtr> m_SplineViews;
    ICurveEditorSplineViewFactory& m_SplineViewFactory;
    EditorListenerHandle m_ListenerHandle;
};

#endif //__CURVE_EDITOR_VIEW_H__