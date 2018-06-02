#include "pch.h"
#include "ComponentToolActionSetter.h"
#include "CurveEditorTool.h"
#include "EditorContext.h"
#include "CurveEditorController.h"
#include <QMessageBox>

CComponentToolActionSetter::CComponentToolActionSetter(QAction& action, const IEditorContext& editorContext, IComponentToolBuilder& componentToolBuilder) :
    QObject(action.parent()),
    m_EditorContext(editorContext),
    m_ComponentToolBuilder(componentToolBuilder)
{
    const auto result = connect(&action, &QAction::triggered, this, &CComponentToolActionSetter::OnTriggered);
    EDITOR_ASSERT(result);
}

void CComponentToolActionSetter::OnTriggered()
{
    auto tool = ICurveEditorComponentToolSharedPtr(ICurveEditorComponentTool::Create());

    const auto buildResult = m_ComponentToolBuilder.Build(*tool);
    EDITOR_ASSERT(buildResult);

    if (!buildResult)
    {
        QMessageBox::critical(nullptr, "Critical error", "Cannot build component tool!");
        return;
    }

    const auto& editorController = m_EditorContext.GetController();
    const auto curveEditorController = dynamic_cast<ICurveEditorController*>(editorController.get());
    EDITOR_ASSERT(curveEditorController);

    if (!curveEditorController)
    {
        QMessageBox::critical(nullptr, "Critical error", "Valid editor controller is not set!");
        return;
    }

    const auto result = curveEditorController->SetActiveTool(std::move(tool));

    if (!result)
    {
        QMessageBox::critical(nullptr, "Critical error", "Cannot set active tool!");
        return;
    }
}