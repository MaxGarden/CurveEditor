#pragma once
#if !defined(__ZOOM_TOOL_BUILDER_H__)

#include "DefaultToolBuilder.h"

class CZoomToolBuilder final : public CDefaultToolBuilder
{
public:
    CZoomToolBuilder() = default;
    virtual ~CZoomToolBuilder() override = default;

    virtual bool Build(ICurveEditorComponentTool& tool) override final;
};

#endif //__ZOOM_TOOL_BUILDER_H__