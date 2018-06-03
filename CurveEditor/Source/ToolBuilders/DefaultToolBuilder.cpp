#include "pch.h"
#include "DefaultToolBuilder.h"
#include "Tools/CurveEditorZoomTool.h"
#include "Tools/CurveEditorScrollTool.h"

bool CDefaultToolBuilder::Build(ICurveEditorComponentTool& tool)
{
    auto result = true;

    result &= tool.AddComponent(std::make_unique<CCurveEditorZoomTool>(ECurveEditorModifier::Control, ECurveEditorModifier::Shift));
    result &= tool.AddComponent(std::make_unique<CCurveEditorScrollTool>(ECurveEditorMouseButton::Right));

    EDITOR_ASSERT(result);
    return result;
}