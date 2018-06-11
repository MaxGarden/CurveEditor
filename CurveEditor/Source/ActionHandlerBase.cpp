#include "pch.h"
#include "ActionHandlerBase.h"
#include <QMessageBox>

CActionHandlerBase::CActionHandlerBase(QAction& action) :
    QObject(action.parent()),
    m_Action(action)
{
    const auto result = connect(&action, &QAction::triggered, this, &CActionHandlerBase::OnTriggered);
    EDITOR_ASSERT(result);
    if(!result)
        QMessageBox::critical(action.parentWidget(), "Critical error", "Cannot connect action trigger");
}

void CActionHandlerBase::OnTriggered()
{
    HandleAction();
}

void CActionHandlerBase::HandleAction()
{
    //to override
}

QAction& CActionHandlerBase::GetAction() const noexcept
{
    return m_Action;
}


CEditorContextActionHandlerBase::CEditorContextActionHandlerBase(QAction& action, IEditorContext& editorContext) :
    CActionHandlerBase(action),
    m_EditorContext(editorContext)
{
}

IEditorContext& CEditorContextActionHandlerBase::GetEditorContext() const noexcept
{
    return m_EditorContext;
}