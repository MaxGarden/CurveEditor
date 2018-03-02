#include "EditorViewWidget.h"
#include "pch.h"
#include <QTimer>

class CEditorViewWidget final : public QOpenGLWidget, private QOpenGLExtraFunctions
{
public:
    CEditorViewWidget(IEditorViewSharedPtr&& editorView, QWidget* parent = nullptr, uint updateTimeMs = 16);
    virtual ~CEditorViewWidget() override;

    IEditorView* GetEditorView() const noexcept;

    const ImVec4& GetClearColor() const noexcept;
    void SetClearColor(const ImVec4& color) noexcept;

protected:
    virtual void initializeGL() override final;
    virtual void paintGL() override final;

private:
    IEditorViewSharedPtr m_EditorView;
    QtImGuiContext m_Context = nullptr;

    ImVec4 m_ClearColor = ImColor(127, 127, 127);
};


CEditorViewWidget::CEditorViewWidget(IEditorViewSharedPtr&& editorView, QWidget* parent /* = nullptr */, uint updateTimeMs /* = 16 */) :
    QOpenGLWidget(parent),
    m_EditorView(std::move(editorView))
{
    assert(m_EditorView);
    if (m_EditorView)
    {
        const auto timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), SLOT(update()));
        timer->start(updateTimeMs);
    }
}

CEditorViewWidget::~CEditorViewWidget()
{
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
    const auto previousContext = QtImGui::GetCurrentContext();
    QtImGui::SetCurrentContext(m_Context);

    QtImGui::BeginFrame();

    if(m_EditorView)
        m_EditorView->OnFrame();

    glViewport(0, 0, width(), height());
    glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    QtImGui::EndFrame();
    QtImGui::SetCurrentContext(previousContext);;
}

class CDefaultViewWidgetFactory final : public IEditorViewWidgetFactory
{
public:
    CDefaultViewWidgetFactory(IEditorViewFactorySharedPtr&& editorViewFactory);
    virtual ~CDefaultViewWidgetFactory() override final = default;

    virtual std::unique_ptr<QWidget> Create(QWidget* parent) override final;

private:
    IEditorViewFactorySharedPtr m_EditorViewFactory;
};

CDefaultViewWidgetFactory::CDefaultViewWidgetFactory(IEditorViewFactorySharedPtr&& editorViewFactory) :
    m_EditorViewFactory(std::move(editorViewFactory))
{
}

std::unique_ptr<QWidget> CDefaultViewWidgetFactory::Create(QWidget* parent)
{
    return m_EditorViewFactory ? std::make_unique<CEditorViewWidget>(m_EditorViewFactory->Create(), parent) : nullptr;
}

IEditorViewWidgetFactoryUniquePtr IEditorViewWidgetFactory::CreateFactory(IEditorViewFactorySharedPtr&& editorViewFactory)
{
    return std::make_unique<CDefaultViewWidgetFactory>(std::move(editorViewFactory));
}