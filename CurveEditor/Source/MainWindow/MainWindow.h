#pragma once
#if !defined(__MAIN_WINDOW_H__)

#include <QMainWindow>
#include <QAction>
#include "ui_MainWindow.h"
#include "EditorWidgets/EditorViewWidget.h"

enum class ESetToolActionType
{
    MovingTool,
    KnotInserter,
    KnotRemover,

    __Count
};

class CMainWindow final : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    CMainWindow(IEditorViewWidgetFactory& curveEditorWidgetFactory);
    virtual ~CMainWindow() override final = default;

    bool Setup();

    QAction* GetSetToolAction(ESetToolActionType actionType) const noexcept;

private slots:
    void OnAddEditorButtonClicked();
    void OnRemoveEditorButtonClicked();

private:
    void AddCurveEditorView();

private:
    IEditorViewWidgetFactory& m_CurveEditorWidgetFactory;
    std::vector<QWidget*> m_EditorsWidgets;
    QActionGroup* m_ToolBarActionGroup = nullptr;
};

#endif //__MAIN_WINDOW_H__