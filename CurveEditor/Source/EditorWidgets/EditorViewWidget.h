#pragma once
#if !defined(__EDITOR_VIEW_WIDGET_H__)

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QtImGui.h>
#include <imgui.h>
#include "EditorView.h"

class IEditorViewWidgetFactory
{
public:
    virtual ~IEditorViewWidgetFactory() = default;

    virtual std::unique_ptr<QWidget> Create(QWidget* parent) = 0;

    static IEditorViewWidgetFactoryUniquePtr CreateFactory(IEditorViewFactorySharedPtr&& editorViewFactory);
};

#endif //__EDITOR_VIEW_WIDGET_H__