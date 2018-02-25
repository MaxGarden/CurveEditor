#pragma once
#if !defined(__EDITOR_VIEW_WIDGET_H__)

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include "EditorView.h"
#include "QtImGui.h"
#include "imgui.h"

class IEditorViewWidgetFactory
{
public:
    virtual ~IEditorViewWidgetFactory() = default;

    virtual std::unique_ptr<QWidget> Create(QWidget* parent) = 0;

    static std::unique_ptr<IEditorViewWidgetFactory> CreateFactory(std::shared_ptr<IEditorViewFactory> editorViewFactory);
};

#endif //__EDITOR_VIEW_WIDGET_H__