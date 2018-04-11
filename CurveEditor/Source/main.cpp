#include "pch.h"
#include "MainWindow/MainWindow.h"
#include "EditorWidgets/EditorViewWidget.h"
#include "CurveEditorFactory.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"
#include "SplineControllerFactory.h"
#include "SplineViewFactory.h"
#include "CurveEditorTool.h"
#include "Tools/CurveEditorScrollTool.h"
#include "Tools/CurveEditorZoomTool.h"
#include "EditorContext.h"

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

    auto controller = ICurveEditorController::Create(splineControllerFactory);

    const auto tool = ICurveEditorComponentToolSharedPtr(ICurveEditorComponentTool::Create());
    tool->AddComponent(std::make_unique<CCurveEditorScrollTool>(ECurveEditorMouseButton::Right));
    tool->AddComponent(std::make_unique<CCurveEditorZoomTool>());

    controller->SetActiveTool(tool);

    curveEditorContext->SetController(std::move(controller));
    curveEditorContext->SetDataModel(ICurveEditorDataModel::Create());

    const auto curveEditorWidgetFactory = IEditorViewWidgetFactory::CreateFactory(curveEditorContext);
    CMainWindow mainWindow(*curveEditorWidgetFactory);

    mainWindow.show();

    return app.exec();
}