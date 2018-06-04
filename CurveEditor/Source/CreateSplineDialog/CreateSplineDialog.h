#pragma once
#if !defined(__CREATE_SPLINE_DIALOG_WINDOW_H__)

#include <QDialog>
#include "ui_CreateSplineDialog.h"

class CCreateSplineDialog final : public QDialog, public Ui::CreateSplineDialog
{
    Q_OBJECT

public:
    CCreateSplineDialog(QWidget* parent = nullptr);
    virtual ~CCreateSplineDialog() override final = default;
};

#endif //__CREATE_SPLINE_DIALOG_WINDOW_H__