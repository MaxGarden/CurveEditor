#pragma  once
#if !defined(__EDITOR_VIEW_H__)

class IEditorView
{
public:
    virtual ~IEditorView() = default;

    virtual void DrawFrame() = 0;
};

#endif //__EDITOR_VIEW_H__