#pragma  once
#if !defined(__CURVE_EDITOR_RULER_COMPONENT_H__)

#include "CurveEditorView.h"

class ICurveEditorRulerViewComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorRulerViewComponent() override = default;

    static ICurveEditorRulerViewComponentUniquePtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_RULER_COMPONENT_H__