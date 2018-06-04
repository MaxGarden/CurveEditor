#include "pch.h"
#include "CreateSplineDialog.h"
#include "CurveEditorDataModel.h"
#include <QColorDialog>
#include <QMessageBox>

CCreateSplineDialog::CCreateSplineDialog(QWidget* parent, ICurveEditorDataModel& editorDataModel) :
    QDialog(parent),
    m_EditorDataModel(editorDataModel)
{
    setupUi(this);
    Setup();
}

void CCreateSplineDialog::Setup()
{
    if (const auto splineTypeComboBox = m_SplineTypeComboBox)
        splineTypeComboBox->addItems({ "Function" });

    OnSelectedSplineColor(m_SplineColor);
}

void CCreateSplineDialog::OnCreateButtonClicked()
{
    auto splineColor = m_SplineColor;
    splineColor.setRed(m_SplineColor.blue());
    splineColor.setBlue(m_SplineColor.red());

    const auto splineDataModel = m_EditorDataModel.AddSplineDataModel(static_cast<SplineColor>(splineColor.rgb()));
    EDITOR_ASSERT(splineDataModel);
    if (!splineDataModel)
    {
        QMessageBox::critical(this, "Error", "Cannot add spline data model.");
        return reject();
    }
    const auto addControlPointsResult = splineDataModel->AddControlPoints({
        { 0, { 1.0f, -3.0f } },
        { 1, { 2.0f, -2.0f } },
        { 2, { 3.0f, -4.0f } },
        { 3, { 4.0f, -3.0f } }
        });

    EDITOR_ASSERT(addControlPointsResult);
    accept();
}

void CCreateSplineDialog::OnChangeColorButtonClicked()
{
    QColorDialog colorDialog{ this };

    const auto result = connect(&colorDialog, SIGNAL(colorSelected(const QColor&)), SLOT(OnSelectedSplineColor(const QColor&)));
    EDITOR_ASSERT(result);

    if(result)
        colorDialog.exec();
}

void CCreateSplineDialog::OnSelectedSplineColor(const QColor& color)
{
    m_SplineColor = color;

    if (const auto splineChangeColorButton = m_ChangeColorButton)
    {
        const auto qss = QString("background-color: %1").arg(m_SplineColor.name());
        splineChangeColorButton->setStyleSheet(qss);
    }
}