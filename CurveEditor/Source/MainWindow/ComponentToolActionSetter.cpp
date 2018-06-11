#include "pch.h"
#include "ComponentToolActionSetter.h"
#include "CurveEditorTool.h"
#include "EditorContext.h"
#include "CurveEditorController.h"
#include <QMessageBox>

CComponentToolActionSetter::CComponentToolActionSetter(QAction& action, IEditorContext& editorContext, IComponentToolBuilder& componentToolBuilder) :
    CEditorContextActionHandlerBase(action, editorContext),
    m_ComponentToolBuilder(componentToolBuilder)
{
}

void CComponentToolActionSetter::HandleAction()
{
    auto tool = ICurveEditorComponentToolSharedPtr(ICurveEditorComponentTool::Create());

    const auto buildResult = m_ComponentToolBuilder.Build(*tool);
    EDITOR_ASSERT(buildResult);

    if (!buildResult)
    {
        QMessageBox::critical(nullptr, "Critical error", "Cannot build component tool!");
        return;
    }

    const auto& editorController = GetEditorContext().GetController();
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