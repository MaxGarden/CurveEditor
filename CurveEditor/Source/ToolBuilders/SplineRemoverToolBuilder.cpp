#include "pch.h"
#include "SplineRemoverToolBuilder.h"
#include "Tools/CurveEditorZoomTool.h"
#include "Tools/CurveEditorHoveringTool.h"
#include "Tools/CurveEditorSplineRemoverTool.h"

bool CSplineRemoverToolBuilder::Build(ICurveEditorComponentTool& tool)
{
    auto result = true;

    result &= tool.AddComponent(std::make_unique<CCurveEditorZoomTool>());
    result &= tool.AddComponent(std::make_unique<CCurveEditorHoveringTool>(std::set<ECurveEditorSplineComponentType>{ ECurveEditorSplineComponentType::Curve }, true));
    result &= tool.AddComponent(std::make_unique<CCurveEditorSplineRemoverTool>(ECurveEditorMouseButton::Left));

    EDITOR_ASSERT(result);
    return result && CDefaultToolBuilder::Build(tool);
}