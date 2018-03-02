#include "pch.h"
#include "MainWindow/MainWindow.h"
#include "EditorWidgets/EditorViewWidget.h"
#include "CurveEditorFactory.h"
#include "EditorContext.h"

int main(int argc, char** argv)
{
    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);

    QApplication app(argc, argv);

    CEditorContext curveEditorContext;

    curveEditorContext.SetViewFactory(std::make_unique<CCurveEditorViewFactory>());

    const auto curveEditorWidgetFactory = IEditorViewWidgetFactory::CreateFactory(std::make_shared<CCurveEditorViewFactory>());
    CMainWindow mainWindow(*curveEditorWidgetFactory);

    mainWindow.show();

    return app.exec();
}