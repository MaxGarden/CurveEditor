#include "pch.h"
#include "CreateSplineDialog.h"
#include "CurveEditorDataModel.h"
#include <QColorDialog>
#include <QMessageBox>

const std::map<QString, ECurveEditorSplineType> CCreateSplineDialog::s_SplineTypesMaps = {
    { "Function", ECurveEditorSplineType::Function },
    { "Path", ECurveEditorSplineType::Path } };

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
    {
        VisitObjectsContainer(s_SplineTypesMaps, [splineTypeComboBox](const auto& pair)
        {
            splineTypeComboBox->addItem(pair.first);
        });
    }

    OnSelectedSplineColor(m_SplineColor);
}

void CCreateSplineDialog::OnCreateButtonClicked()
{
    const auto splineColor = [this]()
    {
        auto result = m_SplineColor;
        result.setRed(m_SplineColor.blue());
        result.setBlue(m_SplineColor.red());

        return result;
    }();

    const auto splineType = [this]()
    {
        const auto splineTypeComboBox = m_SplineTypeComboBox;
        if (!splineTypeComboBox)
            return ECurveEditorSplineType::Function;

        const auto iterator = s_SplineTypesMaps.find(splineTypeComboBox->currentText());
        EDITOR_ASSERT(iterator != s_SplineTypesMaps.cend());
        if(iterator == s_SplineTypesMaps.cend())
            return ECurveEditorSplineType::Function;

        return iterator->second;
    }();

    const auto splineID = m_EditorDataModel.GetFreeSplineID();
    auto splineDataModel = ICurveEditorSplineDataModel::Create(splineID, static_cast<SplineColor>(splineColor.rgb()), splineType);

    const auto result = m_EditorDataModel.AddSplineDataModel(std::move(splineDataModel));
    EDITOR_ASSERT(result);
    if (!result)
    {
        QMessageBox::critical(this, "Error", "Cannot add spline data model.");
        return reject();
    }

    const auto newSplineDataModel = m_EditorDataModel.GetSplineDataModel(splineID);
    EDITOR_ASSERT(newSplineDataModel);
    if (!newSplineDataModel)
        return;

    const auto addControlPointsResult = newSplineDataModel->AddControlPoints({
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
    QColorDialog colorDialog{ m_SplineColor, this };

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