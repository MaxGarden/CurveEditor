#pragma once
#if !defined(__EDITOR_VIEW_H__)

#include "EditorDataModel.h"
#include "EditorController.h"
#include "EditorRenderable.h"

class IEditorView : public IEditorRenderable
{
public:
    virtual ~IEditorView() override = default;

    virtual bool SetController(const IEditorControllerSharedPtr& controller) = 0;
};

class IEditorViewFactory
{
public:
    virtual ~IEditorViewFactory() = default;
    virtual std::unique_ptr<IEditorView> Create() = 0;
};

#endif //__EDITOR_VIEW_H__