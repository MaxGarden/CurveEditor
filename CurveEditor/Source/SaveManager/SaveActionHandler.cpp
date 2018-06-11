#include "pch.h"
#include "SaveActionHandler.h"
#include "EditorContext.h"
#include "SaveManager.h"
#include <QMessageBox>
#include <QFileDialog>

CSaveActionHandler::CSaveActionHandler(QAction& action, IEditorContext& editorContext) :
    CEditorContextActionHandlerBase(action, editorContext)
{
}

void CSaveActionHandler::HandleAction()
{
    auto& saveManager = ISaveManager::GetInstance();
    const auto parentWidget = GetAction().parentWidget();

    if (!saveManager.GetSaveFilePath())
    {
        const auto filename = QFileDialog::getSaveFileName(parentWidget,
            tr("Open"), "", tr("Editor Save (*.xml)"));

        if (filename.isNull())
            return;

        saveManager.SetSaveFilePath(filename.toStdString());
    }

    const auto result = saveManager.Save(GetEditorContext());
    EDITOR_ASSERT(result);
    if(!result)
        QMessageBox::critical(parentWidget, "Error", "Cannot save editor context!");
}