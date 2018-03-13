#pragma  once
#if !defined(__EDITOR_VIEW_H__)

#include "EditorDataModel.h"
#include "EditorController.h"

class IEditorView
{
public:
    virtual ~IEditorView() = default;

    virtual void OnFrame() = 0;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) noexcept = 0;
};

class IEditorViewFactory
{
public:
    virtual ~IEditorViewFactory() = default;
    virtual std::unique_ptr<IEditorView> Create() = 0;
};

#endif //__EDITOR_VIEW_H__