#pragma once
#if !defined(__DEFAULT_TOOL_BUILDER_H__)

#include "ComponentToolBuilder.h"

class CDefaultToolBuilder : public IComponentToolBuilder
{
public:
    CDefaultToolBuilder() = default;
    virtual ~CDefaultToolBuilder() override = default;

    virtual bool Build(ICurveEditorComponentTool& tool) override;
};

#endif //__DEFAULT_TOOL_BUILDER_H__