#pragma  once
#if !defined(__EDITOR_RENDERABLE_H__)

class IEditorRenderable
{
public:
    virtual ~IEditorRenderable() = default;

    virtual void OnFrame() = 0;
};

#endif //__EDITOR_RENDERABLE_H__