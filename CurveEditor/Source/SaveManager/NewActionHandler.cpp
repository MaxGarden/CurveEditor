#include "pch.h"
#include "NewActionHandler.h"
#include "EditorContext.h"
#include "CurveEditorDataModel.h"
#include "CurveEditorSelectionDataModel.h"
#include "SaveManager.h"

CNewActionHandler::CNewActionHandler(QAction& action, IEditorContext& editorContext) :
    CEditorContextActionHandlerBase(action, editorContext)
{
}

void CNewActionHandler::HandleAction()
{
    auto dataModel = ICurveEditorDataModel::Create();
    dataModel->SetSelectionDataModel(ICurveEditorSelectionDataModel::Create());

    auto& editorContext = GetEditorContext();
    editorContext.SetDataModel(std::move(dataModel));

    auto& saveManager = ISaveManager::GetInstance();
    saveManager.SetSaveFilePath(std::nullopt);
}