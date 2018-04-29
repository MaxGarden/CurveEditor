#pragma  once
#if !defined(__CURVE_EDITOR_BACKGROUND_COMPONENT_H__)

#include "CurveEditorView.h"

class ICurveEditorBackgroundViewComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorBackgroundViewComponent() override = default;

    static ICurveEditorBackgroundViewComponentUniquePtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_BACKGROUND_COMPONENT_H__