#pragma once
#if !defined(__CURVE_EDITOR_GRID_COMPONENT_H__)

#include "CurveEditorView.h"

class ICurveEditorGridViewComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorGridViewComponent() override = default;

    static ICurveEditorGridViewComponentUniquePtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_GRID_COMPONENT_H__