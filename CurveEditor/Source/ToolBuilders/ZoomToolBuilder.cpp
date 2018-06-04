#include "pch.h"
#include "ZoomToolBuilder.h"
#include "Tools/CurveEditorZoomTool.h"
#include "Tools/CurveEditorRectNavigationTool.h"
#include "Tools/CurveEditorSplineNavgationTool.h"
#include "Tools/CurveEditorHoveringTool.h"

bool CZoomToolBuilder::Build(ICurveEditorComponentTool& tool)
{
    auto result = true;

    result &= tool.AddComponent(std::make_unique<CCurveEditorZoomTool>(ECurveEditorModifier::Control, ECurveEditorModifier::Shift));
    result &= tool.AddComponent(std::make_unique<CCurveEditorRectNavigationTool>(ECurveEditorMouseButton::Left));
    result &= tool.AddComponent(std::make_unique<CCurveEditorSplineNavigationTool>(ECurveEditorMouseButton::Left));
    result &= tool.AddComponent(std::make_unique<CCurveEditorHoveringTool>(std::set<ECurveEditorSplineComponentType>{ECurveEditorSplineComponentType::Curve}, true));

    EDITOR_ASSERT(result);
    return result && CDefaultToolBuilder::Build(tool);
}