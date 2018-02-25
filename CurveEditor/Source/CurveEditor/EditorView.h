#pragma  once
#if !defined(__EDITOR_VIEW_H__)

#include <memory>

class IEditorView
{
public:
    virtual ~IEditorView() = default;

    virtual void OnFrame() = 0;
};

class IEditorViewFactory
{
public:
    virtual ~IEditorViewFactory() = default;
    virtual std::unique_ptr<IEditorView> Create() = 0;
};

#endif //__EDITOR_VIEW_H__