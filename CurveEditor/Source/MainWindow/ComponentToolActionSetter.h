#pragma once
#if !defined(__COMPONENT_TOOL_ACTION_SETTER_H__)

#include <QObject>
#include <QAction>
#include "ToolBuilders/ComponentToolBuilder.h"

class CComponentToolActionSetter final : public QObject
{
    Q_OBJECT

public:
    CComponentToolActionSetter(QAction& action, const IEditorContext& editorContex, IComponentToolBuilder& componentToolBuilder);
    virtual ~CComponentToolActionSetter() override final = default;

private slots:
    void OnTriggered();

private:
    const IEditorContext& m_EditorContext;
    IComponentToolBuilder& m_ComponentToolBuilder;
};

#endif //__COMPONENT_TOOL_ACTION_SETTER_H__