#include "pch.h"
#include "MainWindow.h"
#include <QDockWidget>

CMainWindow::CMainWindow(IEditorViewWidgetFactory& curveEditorWidgetFactory) :
    m_CurveEditorWidgetFactory(curveEditorWidgetFactory)
{

    setupUi(this);
    centralWidget()->hide();

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
    case ESetToolActionType::ZoomTool:
        return m_ZoomToolAction;
    case ESetToolActionType::SplineRemover:
        return m_RemoveSplineToolAction;
    default:
        EDITOR_ASSERT(false);
        return nullptr;
    };
}

QAction* CMainWindow::GetActionToolAction(EActionToolType actionType) const noexcept
{
    switch (actionType)
    {
    case EActionToolType::CreateSpline:
        return m_CreateSplineAction;
    case EActionToolType::New:
        return m_NewAction;
    case EActionToolType::Open:
        return m_OpenAction;
    case EActionToolType::Save:
        return m_SaveAction;
    case EActionToolType::SaveAs:
        return m_SaveAsAction;
    default:
        EDITOR_ASSERT(false);
        return nullptr;
    }
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
    auto editorWidget = m_CurveEditorWidgetFactory.Create(this);
    assert(editorWidget);
    if (!editorWidget)
        return;

    const auto dockWidget = new QDockWidget(this);
    editorWidget->setParent(dockWidget);
    dockWidget->setWidget(editorWidget.release());

    addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, dockWidget);

    m_EditorsWidgets.emplace_back(dockWidget);
}