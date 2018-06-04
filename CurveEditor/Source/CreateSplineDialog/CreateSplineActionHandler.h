#pragma once
#if !defined(__CREATE_SPLINE_ACTION_HANDLER_H__)

#include "ActionHandlerBase.h"

class CCreateSplineActionHandler final : public CEditorContextActionHandlerBase
{
public:
    CCreateSplineActionHandler(QAction& action, const IEditorContext& editorContext);
    virtual ~CCreateSplineActionHandler() override final = default;

protected:
    virtual void HandleAction() override final;
};

#endif //__CREATE_SPLINE_ACTION_HANDLER_H__