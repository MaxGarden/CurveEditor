#include "pch.h"
#include "MovingToolBuilder.h"
#include "Tools/CurveEditorZoomTool.h"
#include "Tools/CurveEditorMovingTool.h"
#include "Tools/CurveEditorHoveringTool.h"
#include "Tools/CurveEditorSelectionTool.h"

bool CMovingToolBuilder::Build(ICurveEditorComponentTool& tool)
{
    auto result = true;

    result &= tool.AddComponent(std::make_unique<CCurveEditorZoomTool>());
    result &= tool.AddComponent(std::make_unique<CCurveEditorSelectionTool>(ECurveEditorMouseButton::Left, ECurveEditorSplineComponentType::Knot, ECurveEditorModifier::Control, std::map<ECurveEditorModifier, ECurveEditorSplineComponentType>{ { ECurveEditorModifier::Alt, ECurveEditorSplineComponentType::Tangent }}));
    result &= tool.AddComponent(std::make_unique<CCurveEditorHoveringTool>(std::set<ECurveEditorSplineComponentType>{ ECurveEditorSplineComponentType::Knot, ECurveEditorSplineComponentType::Tangent, ECurveEditorSplineComponentType::Curve }, true));
    result &= tool.AddComponent(std::make_unique<CCurveEditorMovingTool>(ECurveEditorMouseButton::Left));

    EDITOR_ASSERT(result);
    return result && CDefaultToolBuilder::Build(tool);
}