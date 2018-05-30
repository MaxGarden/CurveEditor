#include "pch.h"
#include "MainWindow/MainWindow.h"
#include "EditorWidgets/EditorViewWidget.h"
#include "CurveEditorFactory.h"
#include "CurveEditorController.h"
#include "CurveEditorDataModel.h"
#include "SplineControllerFactory.h"
#include "SplineViewImplementation.h"
#include "CurveEditorTool.h"
#include "Tools/CurveEditorScrollTool.h"
#include "Tools/CurveEditorZoomTool.h"
#include "Tools/CurveEditorSelectionTool.h"
#include "Tools/CurveEditorHoveringTool.h"
#include "Tools/CurveEditorMovingTool.h"
#include "Tools/CurveEditorKnotInserterTool.h"
#include "EditorContext.h"
#include "CurveEditorSelectionDataModel.h"

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

    const auto tool = ICurveEditorComponentToolSharedPtr(ICurveEditorComponentTool::Create());
    tool->AddComponent(std::make_unique<CCurveEditorScrollTool>(ECurveEditorMouseButton::Right));
    tool->AddComponent(std::make_unique<CCurveEditorZoomTool>());
    tool->AddComponent(std::make_unique<CCurveEditorSelectionTool>(ECurveEditorMouseButton::Left, ECurveEditorSplineComponentType::Knot, ECurveEditorModifier::Control, std::map<ECurveEditorModifier, ECurveEditorSplineComponentType>{ { ECurveEditorModifier::Alt, ECurveEditorSplineComponentType::Tangent }, { ECurveEditorModifier::Shift, ECurveEditorSplineComponentType::Curve } }));
    tool->AddComponent(std::make_unique<CCurveEditorHoveringTool>(true));
    tool->AddComponent(std::make_unique<CCurveEditorMovingTool>(ECurveEditorMouseButton::Left));
    tool->AddComponent(std::make_unique<CCurveEditorKnotInserterTool>(ECurveEditorMouseButton::Middle));

    auto controller = ICurveEditorController::Create(splineControllerFactory);
    controller->SetActiveTool(tool);

    auto dataModel = ICurveEditorDataModel::Create();
    dataModel->SetSelectionDataModel(ICurveEditorSelectionDataModel::Create());

    curveEditorContext->SetController(std::move(controller));
    curveEditorContext->SetDataModel(std::move(dataModel));

    const auto curveEditorWidgetFactory = IEditorViewWidgetFactory::CreateFactory(curveEditorContext);
    CMainWindow mainWindow(*curveEditorWidgetFactory);

    mainWindow.show();

    return app.exec();
}