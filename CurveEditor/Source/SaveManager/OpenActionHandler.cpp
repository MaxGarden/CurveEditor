#include "pch.h"
#include "OpenActionHandler.h"
#include "EditorContext.h"
#include "SaveManager.h"
#include <QMessageBox>
#include <QFileDialog>

COpenActionHandler::COpenActionHandler(QAction& action, IEditorContext& editorContext) :
    CEditorContextActionHandlerBase(action, editorContext)
{
}

void COpenActionHandler::HandleAction()
{
    auto& saveManager = ISaveManager::GetInstance();
    const auto parentWidget = GetAction().parentWidget();

    const auto filename = QFileDialog::getOpenFileName(parentWidget,
        tr("Open"), saveManager.GetSaveFilePath().value_or("").c_str(), tr("Editor Save (*.xml)"));

    if (filename.isNull())
        return;

    saveManager.SetSaveFilePath(filename.toStdString());
    const auto result = saveManager.Load(GetEditorContext());
    EDITOR_ASSERT(result);
    if(!result)
        QMessageBox::critical(parentWidget, "Error", "Cannot load editor context!");
}