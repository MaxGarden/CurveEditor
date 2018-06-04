#pragma once
#if !defined(__ACTION_HANDLER_BASE_H__)

#include <QObject>
#include <QAction>

class CActionHandlerBase : public QObject
{
    Q_OBJECT

public:
    CActionHandlerBase(QAction& action);
    virtual ~CActionHandlerBase() override = default;

protected:
    virtual void HandleAction();

    QAction& GetAction() const noexcept;

private slots:
    void OnTriggered();

protected:
    QAction& m_Action;
};

class CEditorContextActionHandlerBase : public CActionHandlerBase
{
public:
    CEditorContextActionHandlerBase(QAction& action, const IEditorContext& editorContext);
    virtual ~CEditorContextActionHandlerBase() override = default;

protected:
    const IEditorContext& GetEditorContext() const noexcept;

private:
    const IEditorContext& m_EditorContext;
};

#endif //__ACTION_HANDLER_BASE_H__