#pragma once
#if !defined(__EDITOR_VIEW_WIDGET_H__)

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QtImGui.h>
#include <imgui.h>
#include "EditorContext.h"

class IEditorViewWidgetFactory
{
public:
    virtual ~IEditorViewWidgetFactory() = default;

    virtual std::unique_ptr<QWidget> Create(QWidget* parent) = 0;

    static IEditorViewWidgetFactoryUniquePtr CreateFactory(IEditorContextSharedPtr editorContext);
};

#endif //__EDITOR_VIEW_WIDGET_H__