#pragma  once
#if !defined(__CURVE_EDITOR_SPLINES_COMPONENT_H__)

#include "CurveEditorViewComponent.h"

class CCurveEditorSplinesViewComponent final : public CCurveEditorViewComponentBase
{
public:
    CCurveEditorSplinesViewComponent(ICurveEditorView& editorView, ICurveEditorSplineViewFactory& splineViewFactory);
    virtual ~CCurveEditorSplinesViewComponent() override final = default;

    virtual void OnFrame() override final;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept override;

    bool CreateSplineView(const ICurveEditorSplineControllerSharedPtr& splineController);
    bool DestroySplineView(const ICurveEditorSplineControllerConstSharedPtr& splineController);

protected:
    virtual void OnControllerChanged() override final;

private:
    void VisitSplineViews(const VisitorType<ICurveEditorSplineView>& visitor) noexcept;
    void RecreateSplineViews();

private:
    std::map<ICurveEditorSplineControllerConstSharedPtr, ICurveEditorSplineViewUniquePtr> m_SplineViews;
    ICurveEditorSplineViewFactory& m_SplineViewFactory;

    EditorListenerHandle m_ListenerHandle;
};

#endif //__CURVE_EDITOR_SPLINES_COMPONENT_H__