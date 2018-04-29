#pragma  once
#if !defined(__CURVE_EDITOR_DEBUG_COMPONENT_H__)

#include "CurveEditorView.h"

class ICurveEditorDebugComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorDebugComponent() override = default;

    static ICurveEditorDebugComponentUniquePtr Create(ICurveEditorView& editorView, IEditorContext& editorContext);
};

#endif //__CURVE_EDITOR_DEBUG_COMPONENT_H__