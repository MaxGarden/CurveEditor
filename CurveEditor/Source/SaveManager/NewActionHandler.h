#pragma once
#if !defined(__NEW_ACTION_HANDLER_H__)

#include "ActionHandlerBase.h"

class CNewActionHandler final : public CEditorContextActionHandlerBase
{
public:
    CNewActionHandler(QAction& action, IEditorContext& editorContext);
    virtual ~CNewActionHandler() override final = default;

protected:
    virtual void HandleAction() override final;
};

#endif //__NEW_ACTION_HANDLER_H__