#pragma  once
#if !defined(__KNOT_INSERTER_TOOL_BUILDER_H__)

#include "DefaultToolBuilder.h"

class CKnotInserterToolBuilder final : public CDefaultToolBuilder
{
public:
    CKnotInserterToolBuilder() = default;
    virtual ~CKnotInserterToolBuilder() override = default;

    virtual bool Build(ICurveEditorComponentTool& tool) override final;
};

#endif //__KNOT_INSERTER_TOOL_BUILDER_H__