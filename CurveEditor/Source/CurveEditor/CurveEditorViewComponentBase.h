#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_COMPONENT_BASE_H__)

#include "CurveEditorView.h"

template<typename SuperClass = ICurveEditorViewComponent, typename ControllerType = ICurveEditorController>
class CCurveEditorViewComponentBase : public CEditorViewBase<SuperClass, ControllerType>
{
public:
    CCurveEditorViewComponentBase(ICurveEditorView& editorView);
    virtual ~CCurveEditorViewComponentBase() override = default;

    virtual bool Initialize() override;

    virtual void OnFrame() override;

    virtual ICurveEditorView& GetEditorView() const noexcept override final;

protected:
    using Super = CCurveEditorViewComponentBase<SuperClass, ControllerType>;

private:
    ICurveEditorView& m_EditorView;
};

#include "CurveEditorViewComponentBase.inl"

#endif //__CURVE_EDITOR_VIEW_COMPONENT_BASE_H__