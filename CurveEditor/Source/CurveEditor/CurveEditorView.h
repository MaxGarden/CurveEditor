#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_H__)

#include "Canvas.h"
#include "EditorView.h"

class CCurveEditorView final : public IEditorView
{
public:
    CCurveEditorView() = default;
    virtual ~CCurveEditorView() override final = default;

    virtual void OnFrame() override final;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept override final;
    virtual bool SetDataModel(const IEditorDataModelConstSharedPtr& dataModel) noexcept override final;

    CEditorCanvas& GetCanvas() noexcept;
    const CEditorCanvas& GetCanvas() const noexcept;

    const CCurveEditorDataModelConstSharedPtr& GetDataModel() const noexcept;
    const CCurveEditorControllerSharedPtr& GetController() const noexcept;

    bool AddComponent(ICurveEditorViewComponentUniquePtr&& component);

private:
    CEditorCanvas m_Canvas = CEditorCanvas(ImVec2(100.0f, 100.0f));
    CCurveEditorDataModelConstSharedPtr m_DataModel;
    CCurveEditorControllerSharedPtr m_Controller;

    std::vector<ICurveEditorViewComponentUniquePtr> m_Components;
};

#endif //__CURVE_EDITOR_VIEW_H__