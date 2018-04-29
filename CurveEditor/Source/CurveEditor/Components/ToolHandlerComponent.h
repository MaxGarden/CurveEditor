#pragma  once
#if !defined(__CURVE_EDITOR_TOOL_HANDLER_COMPONENT_H__)

#include "CurveEditorView.h"

class ICurveEditorToolHandlerComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorToolHandlerComponent() override = default;

    static ICurveEditorToolHandlerComponentUniquePtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_TOOL_HANDLER_COMPONENT_H__