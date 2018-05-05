#include "pch.h"
#include "EditorRenderableBase.h"

void CEditorRenderableBase::OnFrame()
{
    const auto drawList = ImGui::GetWindowDrawList();
    EDITOR_ASSERT(drawList);
    if (!drawList)
        return;

    OnFrame(*drawList);
}

void CEditorRenderableBase::OnFrame(ImDrawList&)
{
    //to override
}