#pragma once
#if !defined(__COMPONENT_TOOL_ACTION_SETTER_H__)

#include "ActionHandlerBase.h"
#include "ToolBuilders/ComponentToolBuilder.h"

class CComponentToolActionSetter final : public CEditorContextActionHandlerBase
{
public:
    CComponentToolActionSetter(QAction& action, const IEditorContext& editorContex, IComponentToolBuilder& componentToolBuilder);
    virtual ~CComponentToolActionSetter() override final = default;

protected:
    virtual void HandleAction() override final;

private:
    IComponentToolBuilder& m_ComponentToolBuilder;
};

#endif //__COMPONENT_TOOL_ACTION_SETTER_H__