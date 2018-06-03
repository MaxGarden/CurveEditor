#include "pch.h"
#include "KnotInserterToolBuilder.h"
#include "Tools/CurveEditorZoomTool.h"
#include "Tools/CurveEditorKnotInserterTool.h"

bool CKnotInserterToolBuilder::Build(ICurveEditorComponentTool& tool)
{
    auto result = true;

    result &= tool.AddComponent(std::make_unique<CCurveEditorZoomTool>());
    result &= tool.AddComponent(std::make_unique<CCurveEditorKnotInserterTool>(ECurveEditorMouseButton::Left));

    EDITOR_ASSERT(result);
    return result && CDefaultToolBuilder::Build(tool);
}