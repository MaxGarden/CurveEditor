#pragma once
#if !defined(__MOVING_TOOL_BUILDER_H__)

#include "DefaultToolBuilder.h"

class CMovingToolBuilder final : public CDefaultToolBuilder
{
public:
    CMovingToolBuilder() = default;
    virtual ~CMovingToolBuilder() override = default;

    virtual bool Build(ICurveEditorComponentTool& tool) override final;
};

#endif //__MOVING_TOOL_BUILDER_H__