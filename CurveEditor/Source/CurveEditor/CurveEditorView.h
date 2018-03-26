#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_H__)

#include "Canvas.h"
#include "EditorView.h"

class CCurveEditorViewBase : public IEditorView
{
public:
    CCurveEditorViewBase() = default;
    virtual ~CCurveEditorViewBase() override = default;

    virtual void OnFrame() override;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept override;

    const CCurveEditorControllerSharedPtr& GetController() const noexcept;

protected:
    virtual void OnControllerChanged() noexcept;

private:
    CCurveEditorControllerSharedPtr m_Controller;
};

class CCurveEditorView final : public CCurveEditorViewBase
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

    bool AddView(CCurveEditorViewBaseUniquePtr&& view);

protected:
    virtual void OnControllerChanged() noexcept override final;

private:
    void VisitViews(const std::function<void(CCurveEditorViewBase&)>& visitor) noexcept;
    void VisitSplineViews(const std::function<void(ICurveEditorSplineView&)>& visitor) noexcept;

    void RecreateSplineViews();

    void OnFrameBegin();
    void OnFrameEnd();

private:
    CEditorCanvas m_Canvas = CEditorCanvas(ImVec2(100.0f, 100.0f));

    std::vector<CCurveEditorViewBaseUniquePtr> m_Views;
    std::map<ICurveEditorSplineControllerConstSharedPtr, ICurveEditorSplineViewUniquePtr> m_SplineViews;
    ICurveEditorSplineViewFactory& m_SplineViewFactory;
    EditorProtocolHandle m_ProtocolHandle;
};

#endif //__CURVE_EDITOR_VIEW_H__