#pragma once
#if !defined(__OPEN_ACTION_HANDLER_H__)

#include "ActionHandlerBase.h"

class COpenActionHandler final : public CEditorContextActionHandlerBase
{
public:
    COpenActionHandler(QAction& action, IEditorContext& editorContext);
    virtual ~COpenActionHandler() override final = default;

protected:
    virtual void HandleAction() override final;
};

#endif //__OPEN_ACTION_HANDLER_H__