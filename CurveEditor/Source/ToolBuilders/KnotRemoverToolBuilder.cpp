#include "pch.h"
#include "KnotRemoverToolBuilder.h"
#include "Tools/CurveEditorKnotRemoverTool.h""

bool CKnotRemoverToolBuilder::Build(ICurveEditorComponentTool& tool)
{
    auto result = true;

    result &= tool.AddComponent(std::make_unique<CCurveEditorKnotRemoverTool>(ECurveEditorMouseButton::Left));

    EDITOR_ASSERT(result);
    return result && CDefaultToolBuilder::Build(tool);
}