#pragma  once
#if !defined(__CURVE_EDITOR_NAVIGATION_COMPONENT_H__)

#include "CurveEditorView.h"

class ICurveEditorNavigationComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorNavigationComponent() override = default;

    virtual void ApplyZoom(const ax::pointf& focusPoint, int steps) = 0;

    static ICurveEditorNavigationComponentUniquePtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_NAVIGATION_COMPONENT_H__