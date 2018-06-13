#include "pch.h"
#include "SaveActionHandler.h"
#include "EditorContext.h"
#include "SaveManager.h"
#include <QMessageBox>
#include <QFileDialog>


CSaveActionHandlerBase::CSaveActionHandlerBase(QAction& action, IEditorContext& editorContext) :
    CEditorContextActionHandlerBase(action, editorContext)
{
}

bool CSaveActionHandlerBase::SetSaveFilePath() const
{
    auto& saveManager = ISaveManager::GetInstance();
    const auto parentWidget = GetAction().parentWidget();

    const auto filename = QFileDialog::getSaveFileName(parentWidget,
        tr("Open"), "", tr("Editor Save (*.xml)"));

    if (filename.isNull())
        return false;

    saveManager.SetSaveFilePath(filename.toStdString());
    return true;
}

bool CSaveActionHandlerBase::Save() const
{
    auto& saveManager = ISaveManager::GetInstance();
    const auto parentWidget = GetAction().parentWidget();

    if (!saveManager.GetSaveFilePath() && !SetSaveFilePath())
        return false;

    const auto result = saveManager.Save(GetEditorContext());
    EDITOR_ASSERT(result);
    if (!result)
        QMessageBox::critical(parentWidget, "Error", "Cannot save editor context!");

    return result;
}

CSaveActionHandler::CSaveActionHandler(QAction& action, IEditorContext& editorContext) :
    CSaveActionHandlerBase(action, editorContext)
{
}

void CSaveActionHandler::HandleAction()
{
    Save();
}

CSaveAsActionHandler::CSaveAsActionHandler(QAction& action, IEditorContext& editorContext) :
    CSaveActionHandlerBase(action, editorContext)
{
}

void CSaveAsActionHandler::HandleAction()
{
    if (!SetSaveFilePath())
        return;

    Save();
}