#pragma  once
#if !defined(__EDITOR_CONTEXT_H__)

#include "CurveEditorDataModel.h"

class IEditorContext
{
public:
    virtual ~IEditorContext() = default;

    virtual void SetViewFactory(IEditorViewFactoryUniquePtr&& factory) noexcept = 0;
    virtual const IEditorViewFactoryUniquePtr& GetViewFactory() const noexcept = 0;

    virtual void SetController(IEditorControllerSharedPtr&& controller) noexcept = 0;
    virtual const IEditorControllerSharedPtr& GetController() const noexcept = 0;

    virtual void SetDataModel(IEditorDataModelSharedPtr&& dataModel) noexcept = 0;
    virtual const IEditorDataModelSharedPtr& GetDataModel() const noexcept = 0;

    virtual IEditorViewSharedPtr AddView() = 0;
    virtual bool RemoveView(const IEditorViewSharedPtr& view) = 0;

    static IEditorContextUniquePtr CreateContext();
};

#endif //__EDITOR_CONTEXT_H__