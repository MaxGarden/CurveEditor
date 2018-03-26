#pragma  once
#if !defined(__CURVE_EDITOR_TOOL_H__)

#include "EditorTool.h"

class ICurveEditorTool : public IEditorTool
{
public:
    virtual ~ICurveEditorTool() override = default;

    virtual void OnDragBegin() = 0;
    virtual void OnDragUpdate() = 0;
    virtual void OnDragEnd() = 0;

    virtual void OnMouseMove() = 0;

    virtual void OnWheel() = 0;

    virtual void OnClick() = 0;
    virtual void OnDoubleClick() = 0;

    virtual void OnKeyDown() = 0;
    virtual void OnKeyUp() = 0;
};


#endif //__CURVE_EDITOR_TOOL_H__