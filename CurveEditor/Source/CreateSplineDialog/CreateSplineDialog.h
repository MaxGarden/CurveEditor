#pragma once
#if !defined(__CREATE_SPLINE_DIALOG_WINDOW_H__)

#include <QDialog>
#include "ui_CreateSplineDialog.h"

class CCreateSplineDialog final : public QDialog, public Ui::CreateSplineDialog
{
    Q_OBJECT

public:
    CCreateSplineDialog(QWidget* parent, ICurveEditorDataModel& editorDataModel);
    virtual ~CCreateSplineDialog() override final = default;

private:
    void Setup();

private slots:
    void OnCreateButtonClicked();
    void OnChangeColorButtonClicked();

    void OnSelectedSplineColor(const QColor& color);

private:
    ICurveEditorDataModel& m_EditorDataModel;
    QColor m_SplineColor = { 0, 0, 255 };
};

#endif //__CREATE_SPLINE_DIALOG_WINDOW_H__