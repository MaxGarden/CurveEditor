#pragma  once
#if !defined(__CURVE_EDITOR_VIEW_VISIBLE_COMPONENT_BASE_H__)

#include "CurveEditorViewComponentBase.h"

template<typename SuperClass = ICurveEditorViewComponent, typename ControllerType = ICurveEditorController>
class CCurveEditorViewVisibleComponentBase : public CCurveEditorViewComponentBase<SuperClass, ControllerType>
{
public:
    CCurveEditorViewVisibleComponentBase(ICurveEditorView& editorView);
    virtual ~CCurveEditorViewVisibleComponentBase() override = default;

    virtual void OnFrame() override;

protected:
    virtual void OnFrame(ImDrawList& drawList);
    virtual void OnFrame(ImDrawList& drawList, ControllerType& controller);

    using Super = CCurveEditorViewVisibleComponentBase<SuperClass, ControllerType>;
};

#include "CurveEditorViewVisibleComponentBase.inl"

#endif //__CURVE_EDITOR_VIEW_VISIBLE_COMPONENT_BASE_H__