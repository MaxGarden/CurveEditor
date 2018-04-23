#include "pch.h"
#include "CurveEditorVisibleToolBase.h"

static auto CreateToolHandle() noexcept
{
    static ToolViewHandle handle = 0;
    return ++handle;
}

std::optional<ToolViewHandle> CCurveEditorVisibleToolBase::AddToolView(ICurveEditorView& activeEditorView, ToolViewFactory&& viewFactory, EComponentOrder order)
{
    if (!viewFactory)
        return std::nullopt;

    SToolViewStorage storage{ std::move(viewFactory), order };

    if (const auto viewHandle = activeEditorView.AddViewComponent(storage.ViewFactory(activeEditorView), order))
    {
        const auto viewComponent = activeEditorView.GetViewComponent(*viewHandle);
        EDITOR_ASSERT(viewComponent);
        if (!viewComponent)
            return std::nullopt;

        storage.ViewComponents.emplace_back(std::make_pair(viewComponent, *viewHandle));
    }

    const auto resultHandle = CreateToolHandle();
    m_ToolViews.try_emplace(resultHandle, std::move(storage));

    //TODO add multiple views

    return resultHandle;
}

bool CCurveEditorVisibleToolBase::RemoveToolView(const ToolViewHandle& handle)
{
    const auto iterator = m_ToolViews.find(handle);
    if (iterator == m_ToolViews.end())
        return false;

    const auto& storage = iterator->second;

    for (const auto& componentPair : storage.ViewComponents)
    {
        const auto viewComponent = componentPair.first.lock();
        if(!viewComponent)
            continue;

        const auto& handle = componentPair.second;

        const auto result = viewComponent->GetEditorView().RemoveViewComponent(handle);
        EDITOR_ASSERT(result && "Cannot remove view component");
    }

    m_ToolViews.erase(iterator);

    return false;
}