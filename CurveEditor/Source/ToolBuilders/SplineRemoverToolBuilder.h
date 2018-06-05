#pragma once
#if !defined(__SPLINE_REMOVER_TOOL_BUILDER_H__)

#include "DefaultToolBuilder.h"

class CSplineRemoverToolBuilder final : public CDefaultToolBuilder
{
public:
    CSplineRemoverToolBuilder() = default;
    virtual ~CSplineRemoverToolBuilder() override = default;

    virtual bool Build(ICurveEditorComponentTool& tool) override final;
};

#endif //__SPLINE_REMOVER_TOOL_BUILDER_H__