#pragma once
#if !defined(__CURVE_EDITOR_NAVIGATION_COMPONENT_H__)

#include "CurveEditorView.h"

class ICurveEditorNavigationComponent : public ICurveEditorViewComponent
{
public:
    virtual ~ICurveEditorNavigationComponent() override = default;

    virtual void ApplyZoom(const ax::pointf& focusPoint, int steps, const ax::pointf& axisMultiplier = { 1.0f, 1.0f }, std::optional<float> zoomTimeOverride = std::nullopt) = 0;
    virtual void NavigateTo(const ax::rectf& bounds, std::optional<float> navigationTimeOverride = std::nullopt) = 0;

    static ICurveEditorNavigationComponentUniquePtr Create(ICurveEditorView& editorView);
};

#endif //__CURVE_EDITOR_NAVIGATION_COMPONENT_H__