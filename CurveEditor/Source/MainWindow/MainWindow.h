#pragma  once
#if !defined(__MAIN_WINDOW_H__)

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "EditorWidgets/EditorViewWidget.h"

class CMainWindow final : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    CMainWindow(IEditorViewWidgetFactory& curveEditorWidgetFactory);
    virtual ~CMainWindow() override final = default;


private slots:
    void OnAddEditorButtonClicked();
    void OnRemoveEditorButtonClicked();

private:
    void Setup();
    void AddCurveEditorView();

private:
    IEditorViewWidgetFactory& m_CurveEditorWidgetFactory;
    std::vector<QWidget*> m_EditorsWidgets;
};

#endif //__MAIN_WINDOW_H__