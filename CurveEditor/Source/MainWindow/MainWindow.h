#pragma  once
#include <QMainWindow>
#include "ui_MainWindow.h"

class CMainWindow final : public QMainWindow, public Ui::MainWIndow
{
public:
    CMainWindow() = default;
    virtual ~CMainWindow() override final = default;
};