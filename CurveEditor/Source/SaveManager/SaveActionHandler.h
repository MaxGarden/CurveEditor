#pragma once
#if !defined(__SAVE_ACTION_HANDLER_H__)

#include "ActionHandlerBase.h"

class CSaveActionHandlerBase : public CEditorContextActionHandlerBase
{
public:
    CSaveActionHandlerBase(QAction& action, IEditorContext& editorContext);
    virtual ~CSaveActionHandlerBase() override = default;

protected:
    bool SetSaveFilePath() const;
    bool Save() const;
};

class CSaveActionHandler final : public CSaveActionHandlerBase
{
public:
    CSaveActionHandler(QAction& action, IEditorContext& editorContext);
    virtual ~CSaveActionHandler() override final = default;

protected:
    virtual void HandleAction() override final;
};

class CSaveAsActionHandler final : public CSaveActionHandlerBase
{
public:
    CSaveAsActionHandler(QAction& action, IEditorContext& editorContext);
    virtual ~CSaveAsActionHandler() override final = default;

protected:
    virtual void HandleAction() override final;
};

#endif //__SAVE_ACTION_HANDLER_H__