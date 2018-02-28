#include <QGuiApplication>
#include <QLayout>
#include "MainWindow/MainWindow.h"
#include "EditorWidgets/EditorViewWidget.h"
#include "EditorFactory.h"

int main(int argc, char** argv)
{
    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);

    QApplication app(argc, argv);

    const auto curveEditorWidgetFactory = IEditorViewWidgetFactory::CreateFactory(std::make_shared<CCurveEditorViewFactory>());
    CMainWindow mainWindow(*curveEditorWidgetFactory);

    mainWindow.show();

    return app.exec();
}