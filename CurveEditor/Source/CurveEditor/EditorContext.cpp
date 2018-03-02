#include "pch.h"
#include "EditorContext.h"

void CEditorContext::SetViewFactory(IEditorViewFactoryUniquePtr&& factory) noexcept
{
    m_ViewFactory = std::move(factory);
}

const IEditorViewFactoryUniquePtr& CEditorContext::GetViewFactory() const noexcept
{
    return m_ViewFactory;
}

void CEditorContext::SetController(IEditorControllerSharedPtr&& controller) noexcept
{
    m_Controller = std::move(controller);

    for (const auto& view : m_Views)
    {
        if (!view)
            continue;

        view->SetController(m_Controller);
    }
}

const IEditorControllerSharedPtr& CEditorContext::GetController() const noexcept
{
    return m_Controller;
}

IEditorViewSharedPtr CEditorContext::AddView()
{
    if (!m_ViewFactory || !m_Controller)
        return nullptr;

    const auto editorView = IEditorViewSharedPtr(m_ViewFactory->Create());
    if (!editorView)
        return nullptr;

    auto isVaild = true;
    isVaild &= editorView->SetController(m_Controller);
    isVaild &= editorView->SetDataModel(m_DataModel);

    if (!isVaild)
        return nullptr;

    m_Views.emplace_back(editorView);

    return editorView;
}

bool CEditorContext::RemoveView(const IEditorViewSharedPtr& view)
{
    const auto iterator = std::find(m_Views.begin(), m_Views.end(), view);
    if (iterator == m_Views.end())
        return false;

    m_Views.erase(iterator);
    return true;
}