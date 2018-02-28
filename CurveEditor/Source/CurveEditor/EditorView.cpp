#include "EditorView.h"

void CEditorView::OnFrame()
{
    for (const auto& component : m_Components)
    {
        if (component)
            component->OnFrame();
    }
}

bool CEditorView::AddComponent(std::unique_ptr<IEditorViewComponent>&& component)
{
    if (!component)
        return false;

    m_Components.emplace_back(std::move(component));
    return true;
}