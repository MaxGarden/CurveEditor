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
    virtual bool SetDataModel(const IEditorDataModelConstSharedPtr& dataModel) noexcept override;

    const CCurveEditorDataModelConstSharedPtr& GetDataModel() const noexcept;
    const CCurveEditorControllerSharedPtr& GetController() const noexcept;

private:
    CCurveEditorDataModelConstSharedPtr m_DataModel;
    CCurveEditorControllerSharedPtr m_Controller;
};

class CCurveEditorView final : public CCurveEditorViewBase
{
public:
    CCurveEditorView() = default;
    virtual ~CCurveEditorView() override final = default;

    virtual void OnFrame() override final;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept override;
    virtual bool SetDataModel(const IEditorDataModelConstSharedPtr& dataModel) noexcept override;

    CEditorCanvas& GetCanvas() noexcept;
    const CEditorCanvas& GetCanvas() const noexcept;

    bool AddView(CCurveEditorViewBaseUniquePtr&& view);

private:
    void VisitViews(const std::function<void(CCurveEditorViewBase&)>& visitor) noexcept;

private:
    CEditorCanvas m_Canvas = CEditorCanvas(ImVec2(100.0f, 100.0f));

    std::vector<CCurveEditorViewBaseUniquePtr> m_Views;
};

#endif //__CURVE_EDITOR_VIEW_H__