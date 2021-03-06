#include "EditorViewWidget.h"
#include "pch.h"
#include <QTimer>

class CEditorViewWidget final : public QOpenGLWidget, private QOpenGLExtraFunctions
{
public:
    CEditorViewWidget(const IEditorContextSharedPtr& editorContext, IEditorViewSharedPtr&& editorView, QWidget* parent = nullptr, uint updateTimeMs = 16);
    virtual ~CEditorViewWidget() override final;

    IEditorView* GetEditorView() const noexcept;

    const ImVec4& GetClearColor() const noexcept;
    void SetClearColor(const ImVec4& color) noexcept;

protected:
    virtual void initializeGL() override final;
    virtual void paintGL() override final;

private:
    IEditorContextSharedPtr m_EditorContext;
    IEditorViewSharedPtr m_EditorView;
    QtImGuiContext m_Context = nullptr;

    bool endedFrame = true;

    ImVec4 m_ClearColor = ImColor(127, 127, 127);
};

CEditorViewWidget::CEditorViewWidget(const IEditorContextSharedPtr& editorContext, IEditorViewSharedPtr&& editorView, QWidget* parent /* = nullptr */, uint updateTimeMs /* = 16 */) :
    QOpenGLWidget(parent),
    m_EditorContext(std::move(editorContext)),
    m_EditorView(std::move(editorView))
{
    EDITOR_ASSERT(m_EditorContext);
    EDITOR_ASSERT(m_EditorView);
    if (m_EditorView)
    {
        const auto timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), SLOT(update()));
        timer->start(updateTimeMs);
    }

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

CEditorViewWidget::~CEditorViewWidget()
{
    if (m_EditorContext && m_EditorContext->RemoveView(m_EditorView))
        m_EditorView.reset();
    else
        EDITOR_ASSERT(false && "Cannot remove editor view");

    if (m_Context)
        QtImGui::DestroyContext(m_Context);
}

IEditorView* CEditorViewWidget::GetEditorView() const noexcept
{
    return m_EditorView.get();
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
    if (!endedFrame)
        return;

    endedFrame = false;

    const auto previousContext = QtImGui::GetCurrentContext();
    QtImGui::SetCurrentContext(m_Context);

    QtImGui::BeginFrame();

    if(m_EditorView)
        m_EditorView->OnFrame();

    glViewport(0, 0, width(), height());
    glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    QtImGui::EndFrame();
    QtImGui::SetCurrentContext(previousContext);

    endedFrame = true;
}

class CDefaultViewWidgetFactory final : public IEditorViewWidgetFactory
{
public:
    CDefaultViewWidgetFactory(IEditorContextSharedPtr&& editorContext);
    virtual ~CDefaultViewWidgetFactory() override final = default;

    virtual std::unique_ptr<QWidget> Create(QWidget* parent) override final;

private:
    IEditorContextSharedPtr m_EditorContext;
};

CDefaultViewWidgetFactory::CDefaultViewWidgetFactory(IEditorContextSharedPtr&& editorContext) :
    m_EditorContext(std::move(editorContext))
{
}

std::unique_ptr<QWidget> CDefaultViewWidgetFactory::Create(QWidget* parent)
{
    return m_EditorContext ? std::make_unique<CEditorViewWidget>(m_EditorContext, m_EditorContext->AddView(), parent) : nullptr;
}

IEditorViewWidgetFactoryUniquePtr IEditorViewWidgetFactory::CreateFactory(IEditorContextSharedPtr editorContext)
{
    return std::make_unique<CDefaultViewWidgetFactory>(std::move(editorContext));
}