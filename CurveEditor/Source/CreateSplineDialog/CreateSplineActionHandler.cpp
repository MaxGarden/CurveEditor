#include "pch.h"
#include "CreateSplineActionHandler.h"
#include "CreateSplineDialog.h"
#include "EditorContext.h"
#include "CurveEditorDataModel.h"
#include <QMessageBox>

CCreateSplineActionHandler::CCreateSplineActionHandler(QAction& action, IEditorContext& editorContext) :
    CEditorContextActionHandlerBase(action, editorContext)
{
}

void CCreateSplineActionHandler::HandleAction()
{
    const auto editorDataModel = dynamic_cast<ICurveEditorDataModel*>(GetEditorContext().GetDataModel().get());
    EDITOR_ASSERT(editorDataModel);

    const auto parentWidget = GetAction().parentWidget();

    if (!editorDataModel)
    {
        QMessageBox::critical(parentWidget, "Error", "Cannot get editor data model!");
        return;
    }

    CCreateSplineDialog{ parentWidget, *editorDataModel }.exec();
}