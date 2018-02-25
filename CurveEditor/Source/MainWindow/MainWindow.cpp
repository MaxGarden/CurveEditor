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
}

void CMainWindow::OnAddEditorButtonClicked()
{
    AddCurveEditorView();
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

    layout->addWidget(editorWidget.release());
}