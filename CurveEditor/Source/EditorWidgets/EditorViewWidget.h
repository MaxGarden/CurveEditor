#pragma once
#if !defined(__EDITOR_VIEW_WIDGET_H__)

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include "EditorView.h"
#include "QtImGui.h"
#include "imgui.h"

class CEditorViewWidget final : public QOpenGLWidget, private QOpenGLExtraFunctions
{
public:
    CEditorViewWidget(IEditorView& editorView, QWidget* parent = nullptr);
    virtual ~CEditorViewWidget() override;

    const IEditorView& GetEditorView() const noexcept;

    const ImVec4& GetClearColor() const noexcept;
    void SetClearColor(const ImVec4& color) noexcept;

protected:
    virtual void initializeGL() override final;
    virtual void paintGL() override final;

private:
    IEditorView& m_EditorView;
    QtImGuiContext m_Context = nullptr;

    ImVec4 m_ClearColor = ImColor(127, 127, 127);
};

#endif //__EDITOR_VIEW_WIDGET_H__