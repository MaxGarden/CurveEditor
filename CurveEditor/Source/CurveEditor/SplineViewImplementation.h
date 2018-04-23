#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_FACTORY_H__)

#include "SplineView.h"
#include "KnotViewComponent.h"
#include "CurveViewComponent.h"
#include "TangentViewComponent.h"

class CCurveEditorSplineView final : public CEditorViewBase<ICurveEditorSplineView, ICurveEditorSplineController>
{
public:
    CCurveEditorSplineView(ICurveEditorView& editorView);
    virtual ~CCurveEditorSplineView() override final = default;

    virtual void OnFrame() override final;

    virtual void VisitSplineComponents(const VisitorType<ICurveEditorSplineViewComponent>& visitor) const noexcept override final;

protected:
    virtual void OnFrame(ICurveEditorSplineController& controller);
    virtual void OnControllerChanged() override final;

private:
    void EnsureCurvesViews(ICurveEditorSplineController& controller);
    void EnsureKnotsViews(ICurveEditorSplineController& controller);
    void EnsureTangentsViews(ICurveEditorSplineController& controller);

    void VisitCurvesViews(const VisitorType<CCurveEditorCurveView>& visitor) noexcept;
    void VisitKnotsViews(const VisitorType<CCurveEditorKnotView>& visitor) noexcept;
    void VisitTangentsViews(const VisitorType<CCurveEditorTangentView>& visitor) noexcept;

private:
    ICurveEditorView& m_EditorView;
    std::vector<CCurveEditorCurveViewUniquePtr> m_CurvesViews;
    std::vector<CCurveEditorKnotViewUniquePtr> m_KnotsViews;
    std::vector<CCurveEditorTangentViewUniquePtr> m_TangentsViews;
};

class CCurveEditorSplineViewFactory final : public ICurveEditorSplineViewFactory
{
public:
    CCurveEditorSplineViewFactory() = default;
    virtual ~CCurveEditorSplineViewFactory() override final = default;

    virtual ICurveEditorSplineViewUniquePtr Create(ICurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr& splineController) override final;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_FACTORY_H__