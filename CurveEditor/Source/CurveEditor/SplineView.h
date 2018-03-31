#pragma  once
#if !defined(__CURVE_EDITOR_SPLINE_VIEW_H__)

#include "SplineController.h"
#include "EditorViewBase.h"
#include "Style.h"

class ICurveEditorSplineView : public IEditorView
{
public:
    virtual ~ICurveEditorSplineView() override = default;
};

class ICurveEditorSplineViewFactory
{
public:
    virtual ~ICurveEditorSplineViewFactory() = default;

    virtual ICurveEditorSplineViewUniquePtr Create(CCurveEditorView& editorView, const ICurveEditorSplineControllerSharedPtr& splineController) = 0;
};

class CCurveEditorSplineViewBase : public CEditorViewBase<ICurveEditorSplineView, ICurveEditorSplineController>
{
public:
    CCurveEditorSplineViewBase(CCurveEditorView& editorView);
    virtual ~CCurveEditorSplineViewBase() override = default;

    virtual void OnFrame() override;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller);

    CCurveEditorView& GetEditorView() noexcept;
    const CCurveEditorView& GetEditorView() const noexcept;
    const SCurveEditorStyle& GetEditorStyle() const noexcept;

private:
    CCurveEditorView& m_EditorView;
};

class CCurveEditorKnotView final : public CCurveEditorSplineViewBase
{
public:
    CCurveEditorKnotView(CCurveEditorView& editorView, size_t knotIndex);
    virtual ~CCurveEditorKnotView() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller) override final;

private:
    std::optional<ax::pointf> GetPosition() const noexcept;
    std::optional<ax::pointf> GetEditorPosition() const noexcept;
    std::optional<ax::rectf> GetBounds() const noexcept;

private:
    size_t m_KnotIndex = 0;
};

class CCurveEditorCurveView final : public CCurveEditorSplineViewBase
{
public:
    CCurveEditorCurveView(CCurveEditorView& editorView, size_t curveIndex);
    virtual ~CCurveEditorCurveView() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller) override final;

private:
    using ControlPoints = std::array<ax::pointf, 4>;

    std::optional<ControlPoints> GetControlPointsPositions() const noexcept;
    std::optional<ControlPoints> GetEditorControlPointsPositions() const noexcept;

private:
    size_t m_CurveIndex = 0;
};

class CCurveEditorSplineView final : public CCurveEditorSplineViewBase
{
public:
    CCurveEditorSplineView(CCurveEditorView& editorView);
    virtual ~CCurveEditorSplineView() override final = default;

protected:
    virtual void OnFrame(ImDrawList& drawList, ICurveEditorSplineController& controller) override final;
    virtual void OnControllerChanged() override final;

private:
    void EnsureCurvesViews(ICurveEditorSplineController& controller);
    void EnsureKnotsViews(ICurveEditorSplineController& controller);
    void VisitCurveViews(const std::function<void(CCurveEditorCurveView&)>& visitor) noexcept;
    void VisitKnotViews(const std::function<void(CCurveEditorKnotView&)>& visitor) noexcept;

private:
    std::vector<CCurveEditorCurveViewUniquePtr> m_CurvesViews;
    std::vector<CCurveEditorKnotViewUniquePtr> m_KnotsViews;
};

#endif //__CURVE_EDITOR_SPLINE_VIEW_H__