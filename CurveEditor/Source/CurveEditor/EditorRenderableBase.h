#pragma  once
#if !defined(__EDITOR_RENDERABLE_BASE_H__)

#include "EditorRenderable.h"

class CEditorRenderableBase : public IEditorRenderable
{
public:
    CEditorRenderableBase() = default;
    virtual ~CEditorRenderableBase() override = default;

    virtual void OnFrame() override final;

protected:
    virtual void OnFrame(ImDrawList& drawList);
};

#endif //__EDITOR_RENDERABLE_BASE_H__