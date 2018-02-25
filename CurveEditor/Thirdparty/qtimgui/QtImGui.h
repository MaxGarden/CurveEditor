#pragma once

class QWidget;
class QWindow;

using QtImGuiContext = void*;

namespace QtImGui {

#ifdef QT_WIDGETS_LIB
    QtImGuiContext CreateContext(QWidget *window);
#endif

    QtImGuiContext CreateContext(QWindow *window);
    void DestroyContext(QtImGuiContext context);

    QtImGuiContext GetCurrentContext();
    void SetCurrentContext(QtImGuiContext context);

    void BeginFrame();
    void EndFrame();
}
