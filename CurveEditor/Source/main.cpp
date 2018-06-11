#include "pch.h"
#include "MainWindow/MainWindow.h"
#include "EditorWidgets/EditorViewWidget.h"
#include "CurveEditorFactory.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"
#include "SplineControllerFactory.h"
#include "SplineViewImplementation.h"
#include "EditorContext.h"
#include "CurveEditorSelectionDataModel.h"

#include "MainWindow/ComponentToolActionSetter.h"
#include "ToolBuilders/MovingToolBuilder.h"
#include "ToolBuilders/KnotInserterToolBuilder.h"
#include "ToolBuilders/KnotRemoverToolBuilder.h"
#include "ToolBuilders/ZoomToolBuilder.h"
#include "ToolBuilders/SplineRemoverToolBuilder.h"

#include "CreateSplineDialog/CreateSplineActionHandler.h"

#include "SaveManager/NewActionHandler.h"
#include "SaveManager/OpenActionHandler.h"
#include "SaveManager/SaveActionHandler.h"
#include "SaveManager/SaveManager.h"

#include "Serializers/CurveEditorDataModelSerializer.h"

int main(int argc, char** argv)
{
    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);

    QApplication app(argc, argv);

    const auto curveEditorContext = IEditorContextSharedPtr(IEditorContext::CreateContext());
    if (!curveEditorContext)
        return -1;

    CCurveEditorSplineControllerFactory splineControllerFactory;
    CCurveEditorSplineViewFactory splineViewFactory;

    curveEditorContext->SetViewFactory(std::make_unique<CCurveEditorViewFactory>(*curveEditorContext, splineViewFactory));
    curveEditorContext->SetController(ICurveEditorController::Create(splineControllerFactory));

    const auto curveEditorWidgetFactory = IEditorViewWidgetFactory::CreateFactory(curveEditorContext);
    CMainWindow mainWindow(*curveEditorWidgetFactory);

    const auto createComponentToolActionSetter = [&mainWindow, &curveEditorContext](auto setToolActionType, auto& builder)
    {
        const auto setAction = mainWindow.GetSetToolAction(setToolActionType);
        EDITOR_ASSERT(setAction);
        if (!setAction)
            return;

        new CComponentToolActionSetter{ *setAction, *curveEditorContext, builder };
    };

    auto& saveManager = ISaveManager::GetInstance();
    saveManager.SetSerializer(ICurveEditorDataModelSerializer::Create());

    CMovingToolBuilder movingToolBuilder;
    createComponentToolActionSetter(ESetToolActionType::MovingTool, movingToolBuilder);

    CKnotInserterToolBuilder knotInserterToolBuilder;
    createComponentToolActionSetter(ESetToolActionType::KnotInserter, knotInserterToolBuilder);

    CKnotRemoverToolBuilder knotRemoverToolBuilder;
    createComponentToolActionSetter(ESetToolActionType::KnotRemover, knotRemoverToolBuilder);

    CZoomToolBuilder zoomToolBuilder;
    createComponentToolActionSetter(ESetToolActionType::ZoomTool, zoomToolBuilder);

    CSplineRemoverToolBuilder splineRemoverToolBuilder;
    createComponentToolActionSetter(ESetToolActionType::SplineRemover, splineRemoverToolBuilder);

    new CCreateSplineActionHandler(*mainWindow.GetActionToolAction(EActionToolType::CreateSpline), *curveEditorContext);
    new CSaveActionHandler(*mainWindow.GetActionToolAction(EActionToolType::Save), *curveEditorContext);
    new COpenActionHandler(*mainWindow.GetActionToolAction(EActionToolType::Open), *curveEditorContext);
    new CNewActionHandler(*mainWindow.GetActionToolAction(EActionToolType::New), *curveEditorContext);
    mainWindow.GetActionToolAction(EActionToolType::New)->trigger();

    if (!mainWindow.Setup())
        return -2;

    mainWindow.show();

    return app.exec();
}