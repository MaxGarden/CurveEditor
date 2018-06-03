#include "pch.h"
#include "ZoomToolBuilder.h"
#include "Tools/CurveEditorRectNavigationTool.h"

bool CZoomToolBuilder::Build(ICurveEditorComponentTool& tool)
{
    auto result = true;

    result &= tool.AddComponent(std::make_unique<CCurveEditorRectNavigationTool>(ECurveEditorMouseButton::Left));

    EDITOR_ASSERT(result);
    return result && CDefaultToolBuilder::Build(tool);
}