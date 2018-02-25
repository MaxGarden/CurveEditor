#include "EditorViewWidget.h"

CEditorViewWidget::CEditorViewWidget(IEditorView& editorView, QWidget* parent /* = nullptr */) :
    QOpenGLWidget(parent),
    m_EditorView(editorView)
{
}

CEditorViewWidget::~CEditorViewWidget()
{
    if (m_Context)
        QtImGui::DestroyContext(m_Context);
}

const IEditorView& CEditorViewWidget::GetEditorView() const noexcept
{
    return m_EditorView;
}

const ImVec4& CEditorViewWidget::GetClearColor() const noexcept
{
    return m_ClearColor;
}

void CEditorViewWidget::SetClearColor(const ImVec4& color) noexcept
{
    m_ClearColor = color;
}

void CEditorViewWidget::initializeGL()
{
    initializeOpenGLFunctions();
    m_Context = QtImGui::CreateContext(this);
}

void CEditorViewWidget::paintGL()
{
    const auto previousContext = QtImGui::GetCurrentContext();
    QtImGui::SetCurrentContext(m_Context);

    QtImGui::BeginFrame();

    m_EditorView.DrawFrame();

    glViewport(0, 0, width(), height());
    glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    QtImGui::EndFrame();
    QtImGui::SetCurrentContext(previousContext);;
}