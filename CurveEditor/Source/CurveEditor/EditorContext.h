#pragma  once
#if !defined(__EDITOR_CONTEXT_H__)

#include "CurveEditorDataModel.h"

class CEditorContext
{
public:
    CEditorContext() = default;
    ~CEditorContext() = default;

    void SetViewFactory(IEditorViewFactoryUniquePtr&& factory) noexcept;
    const IEditorViewFactoryUniquePtr& GetViewFactory() const noexcept;

    void SetController(IEditorControllerSharedPtr&& controller) noexcept;
    const IEditorControllerSharedPtr& GetController() const noexcept;

    IEditorViewSharedPtr AddView();
    bool RemoveView(const IEditorViewSharedPtr& view);

private:
    IEditorDataModelSharedPtr m_DataModel;
    IEditorControllerSharedPtr m_Controller;

    std::vector<IEditorViewSharedPtr> m_Views;

    IEditorViewFactoryUniquePtr m_ViewFactory;
};

#endif //__EDITOR_CONTEXT_H__