#pragma once
#if !defined(__SAVE_ACTION_HANDLER_H__)

#include "ActionHandlerBase.h"

class CSaveActionHandler final : public CEditorContextActionHandlerBase
{
public:
    CSaveActionHandler(QAction& action, IEditorContext& editorContext);
    virtual ~CSaveActionHandler() override final = default;

protected:
    virtual void HandleAction() override final;
};

#endif //__SAVE_ACTION_HANDLER_H__