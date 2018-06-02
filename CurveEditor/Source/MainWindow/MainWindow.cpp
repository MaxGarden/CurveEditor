#include "MainWindow.h"

CMainWindow::CMainWindow(IEditorViewWidgetFactory& curveEditorWidgetFactory) :
    m_CurveEditorWidgetFactory(curveEditorWidgetFactory)
{
    setupUi(this);
    Setup();
    AddCurveEditorView();
}

bool CMainWindow::Setup()
{
    m_ToolBarActionGroup = new QActionGroup(this);

    const auto actionsCount = static_cast<size_t>(ESetToolActionType::__Count);
    for (auto i = 0u; i < actionsCount; ++i)
        m_ToolBarActionGroup->addAction(GetSetToolAction(static_cast<ESetToolActionType>(i)));

    m_ToolBarActionGroup->setExclusive(true);

    const auto firstAction = GetSetToolAction(static_cast<ESetToolActionType>(0u));
    EDITOR_ASSERT(firstAction);
    if (firstAction)
        firstAction->activate(QAction::Trigger);

    return true;
}

QAction* CMainWindow::GetSetToolAction(ESetToolActionType actionType) const noexcept
{
    switch (actionType)
    {
    case ESetToolActionType::MovingTool:
        return m_MovingToolAction;
    case ESetToolActionType::KnotInserter:
        return m_KnotInserterToolAction;
    case ESetToolActionType::KnotRemover:
        return m_KnotRemoverToolAction;
    default:
        EDITOR_ASSERT(false);
        return nullptr;
    };
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