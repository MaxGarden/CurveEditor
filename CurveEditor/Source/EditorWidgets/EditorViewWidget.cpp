#include "EditorViewWidget.h"
#include <QTimer>

class CEditorViewWidget final : public QOpenGLWidget, private QOpenGLExtraFunctions
{
public:
    CEditorViewWidget(std::shared_ptr<IEditorView> editorView, QWidget* parent = nullptr, uint updateTimeMs = 16);
    virtual ~CEditorViewWidget() override;

    IEditorView* GetEditorView() const noexcept;

    const ImVec4& GetClearColor() const noexcept;
    void SetClearColor(const ImVec4& color) noexcept;

protected:
    virtual void initializeGL() override final;
    virtual void paintGL() override final;

private:
    std::shared_ptr<IEditorView> m_EditorView;
    QtImGuiContext m_Context = nullptr;

    ImVec4 m_ClearColor = ImColor(127, 127, 127);
};


CEditorViewWidget::CEditorViewWidget(std::shared_ptr<IEditorView> editorView, QWidget* parent /* = nullptr */, uint updateTimeMs /* = 16 */) :
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
    CDefaultViewWidgetFactory(std::shared_ptr<IEditorViewFactory> editorViewFactory);
    virtual ~CDefaultViewWidgetFactory() override final = default;

    virtual std::unique_ptr<QWidget> Create(QWidget* parent) override final;

private:
    std::shared_ptr<IEditorViewFactory> m_EditorViewFactory;
};

CDefaultViewWidgetFactory::CDefaultViewWidgetFactory(std::shared_ptr<IEditorViewFactory> editorViewFactory) :
    m_EditorViewFactory(editorViewFactory)
{
}

std::unique_ptr<QWidget> CDefaultViewWidgetFactory::Create(QWidget* parent)
{
    return m_EditorViewFactory ? std::make_unique<CEditorViewWidget>(m_EditorViewFactory->Create(), parent) : nullptr;
}

std::unique_ptr<IEditorViewWidgetFactory> IEditorViewWidgetFactory::CreateFactory(std::shared_ptr<IEditorViewFactory> editorViewFactory)
{
    return std::make_unique<CDefaultViewWidgetFactory>(editorViewFactory);
}