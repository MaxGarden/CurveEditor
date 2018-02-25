#include "QtImGui.h"
#include "ImGuiRenderer.h"
#include <QWindow>
#ifdef QT_WIDGETS_LIB
#include <QWidget>
#endif

namespace QtImGui {

#ifdef QT_WIDGETS_LIB

namespace {

class QWidgetWindowWrapper : public WindowWrapper {
public:
    QWidgetWindowWrapper(QWidget *w) : w(w) {}
    void installEventFilter(QObject *object) override {
        return w->installEventFilter(object);
    }
    QSize size() const override {
        return w->size();
    }
    qreal devicePixelRatio() const override {
        return w->devicePixelRatioF();
    }
    bool isActive() const override {
        return w->isActiveWindow();
    }
    QPoint mapFromGlobal(const QPoint &p) const override {
        return w->mapFromGlobal(p);
    }
private:
    QWidget *w;
};
}

QtImGuiContext CreateContext(QWidget *window)
{
    const auto result = new ImGuiRenderer();
    result->initialize(new QWidgetWindowWrapper(window));

    return result;
}

#endif

namespace {

class QWindowWindowWrapper : public WindowWrapper {
public:
    QWindowWindowWrapper(QWindow *w) : w(w) {}
    void installEventFilter(QObject *object) override {
        return w->installEventFilter(object);
    }
    QSize size() const override {
        return w->size();
    }
    qreal devicePixelRatio() const override {
        return w->devicePixelRatio();
    }
    bool isActive() const override {
        return w->isActive();
    }
    QPoint mapFromGlobal(const QPoint &p) const override {
        return w->mapFromGlobal(p);
    }
private:
    QWindow *w;
};

}

QtImGuiContext CreateContext(QWindow *window)
{
    const auto result = new ImGuiRenderer();
    result->initialize(new QWindowWindowWrapper(window));

    return result;
}

void DestroyContext(QtImGuiContext context)
{
    const auto renderer = static_cast<ImGuiRenderer*>(context);
    if (!renderer)
        return;

    renderer->deleteLater();
    delete renderer;
}

static ImGuiRenderer* currentContext = nullptr;

void SetCurrentContext(QtImGuiContext context)
{
    currentContext = static_cast<ImGuiRenderer*>(context);
}

QtImGuiContext GetCurrentContext()
{
    return currentContext;
}

void BeginFrame()
{
    assert(currentContext);
    if (!currentContext)
        return;

    currentContext->beginFrame();
}

void EndFrame()
{
    assert(currentContext);
    if (!currentContext)
        return;

    currentContext->endFrame();
}

}
