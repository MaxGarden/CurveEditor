#pragma once
#if !defined(__KNOT_REMOVER_TOOL_BUILDER_H__)

#include "DefaultToolBuilder.h"

class CKnotRemoverToolBuilder final : public CDefaultToolBuilder
{
public:
    CKnotRemoverToolBuilder() = default;
    virtual ~CKnotRemoverToolBuilder() override = default;

    virtual bool Build(ICurveEditorComponentTool& tool) override final;
};

#endif //__KNOT_REMOVER_TOOL_BUILDER_H__