#pragma once
#if !defined(__COMPONENT_TOOL_BUILDER_H__)

#include "CurveEditorTool.h"

class IComponentToolBuilder
{
public:
    virtual ~IComponentToolBuilder() = default;

    virtual bool Build(ICurveEditorComponentTool& tool) = 0;
};

#endif //__COMPONENT_TOOL_BUILDER_H__