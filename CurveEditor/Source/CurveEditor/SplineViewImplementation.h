#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_FACTORY_H__)

#include "SplineView.h"
#include "EditorViewBase.h"
#include "KnotViewComponent.h"
#include "CurveViewComponent.h"
#include "TangentViewComponent.h"

class CCurveEditorSplineView final : public CEditorViewBase<ICurveEditorSplineView, ICurveEditorSplineController>
{
public:
    CCurveEditorSplineView(ICurveEditorView& editorView);
    virtual ~CCurveEditorSplineView() override final = default;

    virtual void OnFrame() override final;

    virtual void VisitSplineComponents(ECurveEditorSplineComponentType componentType, const InterruptibleVisitorType<ICurveEditorSplineViewComponent>& visitor, bool reverse = false) const noexcept override final;

protected:
    virtual void OnControllerChanged() override final;

private:
    void OnFrame(ICurveEditorSplineController& controller);

    void VisitCurvesViews(const InterruptibleVisitorType<ICurveEditorCurveView>& visitor, bool reverse = false) const noexcept;
    void VisitKnotsViews(const InterruptibleVisitorType<ICurveEditorKnotView>& visitor, bool reverse = false) const noexcept;
    void VisitTangentsViews(const InterruptibleVisitorType<ICurveEditorTangentView>& visitor, bool reverse = false) const noexcept;

    void EnsureCurvesViews(ICurveEditorSplineController& controller);
    void EnsureKnotsViews(ICurveEditorSplineController& controller);
    void EnsureTangentsViews(ICurveEditorSplineController& controller);

private:
    ICurveEditorView& m_EditorView;
    std::vector<ICurveEditorCurveViewSharedPtr> m_CurvesViews;
    std::vector<ICurveEditorKnotViewSharedPtr> m_KnotsViews;
    std::vector<ICurveEditorTangentViewSharedPtr> m_TangentsViews;
};

class CCurveEditorSplineViewFactory final : public ICurveEditorSplineViewFactory
{
public:
    CCurveEditorSplineViewFactory() = default;
    virtual ~CCurveEditorSplineViewFactory() override final = default;

    virtual ICurveEditorSplineViewUniquePtr Create(ICurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr& splineController) override final;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_FACTORY_H__