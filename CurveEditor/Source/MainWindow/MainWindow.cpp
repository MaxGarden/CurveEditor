#include "MainWindow.h"

CMainWindow::CMainWindow(IEditorViewWidgetFactory& curveEditorWidgetFactory) :
    m_CurveEditorWidgetFactory(curveEditorWidgetFactory)
{
    setupUi(this);
    Setup();
    AddCurveEditorView();
}

void CMainWindow::Setup()
{
    if (const auto button = m_AddEditorButton)
        connect(button, SIGNAL(clicked()), SLOT(OnAddEditorButtonClicked()));

    if (const auto button = m_RemoveEditorButton)
        connect(button, SIGNAL(clicked()), SLOT(OnRemoveEditorButtonClicked()));
}

void CMainWindow::OnAddEditorButtonClicked()
{
    AddCurveEditorView();
}

void CMainWindow::OnRemoveEditorButtonClicked()
{
    if (m_EditorsWidgets.empty())
        return;

    if (const auto editorWidget = m_EditorsWidgets.back())
        editorWidget->deleteLater();

    m_EditorsWidgets.pop_back();
}

void CMainWindow::AddCurveEditorView()
{
    assert(m_EditorsGroupBox);
    if (!m_EditorsGroupBox)
        return;

    const auto layout = m_EditorsGroupBox->layout();
    assert(layout);

    if (!layout)
        return;

    auto editorWidget = m_CurveEditorWidgetFactory.Create(this);
    assert(editorWidget);
    if (!editorWidget)
        return;

    m_EditorsWidgets.emplace_back(editorWidget.release());
    layout->addWidget(m_EditorsWidgets.back());
}