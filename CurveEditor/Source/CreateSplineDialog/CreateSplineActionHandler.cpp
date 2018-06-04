#include "pch.h"
#include "CreateSplineActionHandler.h"
#include "CreateSplineDialog.h"
#include "EditorContext.h"
#include "CurveEditorDataModel.h"
#include <QMessageBox>

CCreateSplineActionHandler::CCreateSplineActionHandler(QAction& action, const IEditorContext& editorContext) :
    CEditorContextActionHandlerBase(action, editorContext)
{
}

void CCreateSplineActionHandler::HandleAction()
{
    const auto editorDataModel = dynamic_cast<ICurveEditorDataModel*>(GetEditorContext().GetDataModel().get());
    EDITOR_ASSERT(editorDataModel);

    if (!editorDataModel)
    {
        QMessageBox::critical(GetAction().parentWidget(), "Error", "Cannot get editor data model!");
        return;
    }

    CCreateSplineDialog{ GetAction().parentWidget(), *editorDataModel }.exec();
}