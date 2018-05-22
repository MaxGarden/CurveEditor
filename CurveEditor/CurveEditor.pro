CONFIG -= flat
TEMPLATE = app
TARGET = CurveEditor
BINDIR = $$OUT_PWD/bin
DESTDIR = $$BINDIR
QT += core gui widgets

CONFIG += precompile_header suppress_vcproj_warnings
QMAKE_CXXFLAGS += /std:c++latest

PRECOMPILED_HEADER = Source/pch.h

RESOURCES += Source/MainWindow/Resources/MainWindow.qrc

include(Thirdparty/qtimgui/qtimgui.pri)

INCLUDEPATH += $$PWD/Thirdparty/ $$PWD/Thirdparty/qtimgui/imgui $$PWD/Source/CurveEditor $$PWD/Source

SOURCES += \
    Source/main.cpp \
    Source/MainWindow/MainWindow.cpp                            \
    Source/CurveEditor/CurveEditorView.cpp                      \
    Source/CurveEditor/CurveEditorDataModel.cpp                 \
    Source/CurveEditor/CurveEditorController.cpp                \
    Source/CurveEditor/CurveEditorListenerBase.cpp              \
    Source/CurveEditor/CurveEditorSplineListenerBase.cpp        \
    Source/CurveEditor/CurveEditorSelectionListenerBase.cpp     \
    Source/CurveEditor/CurveEditorFactory.cpp                   \
    Source/CurveEditor/CurveEditorSelectionDataModel.cpp        \
    Source/CurveEditor/CurveEditorSelectionController.cpp       \
    Source/CurveEditor/CurveEditorSelectionView.cpp             \
    Source/CurveEditor/SplineDataModel.cpp                      \
    Source/CurveEditor/KnotController.cpp                       \
    Source/CurveEditor/TangentController.cpp                    \
    Source/CurveEditor/CurveController.cpp                      \
    Source/CurveEditor/SplineController.cpp                     \
    Source/CurveEditor/SplineControllerFactory.cpp              \
    Source/CurveEditor/SplineFunctionController.cpp             \
    Source/CurveEditor/SplineView.cpp                           \
    Source/CurveEditor/SplineComponentView.cpp                  \
    Source/CurveEditor/SplineViewImplementation.cpp             \
    Source/CurveEditor/CurveViewComponent.cpp                   \
    Source/CurveEditor/TangentViewComponent.cpp                 \
    Source/CurveEditor/KnotViewComponent.cpp                    \
    Source/CurveEditor/EditorContext.cpp                        \
    Source/CurveEditor/EditorRenderable.cpp                     \
    Source/CurveEditor/EditorRenderableBase.cpp                 \
    Source/CurveEditor/EditorView.cpp                           \
    Source/CurveEditor/EditorController.cpp                     \
    Source/CurveEditor/EditorListener.cpp                       \
    Source/CurveEditor/EditorTool.cpp                           \
    Source/CurveEditor/CurveEditorToolEvent.cpp                 \
    Source/CurveEditor/CurveEditorTool.cpp                      \
    Source/CurveEditor/EditorDataModel.cpp                      \
    Source/CurveEditor/Utilities.cpp                            \
    Source/CurveEditor/Style.cpp                                \
    Source/CurveEditor/Canvas.cpp                               \
    Source/CurveEditor/Components/GridComponent.cpp             \
    Source/CurveEditor/Components/BackgroundComponent.cpp       \
    Source/CurveEditor/Components/DebugComponent.cpp            \
    Source/CurveEditor/Components/ToolHandlerComponent.cpp      \
    Source/CurveEditor/Components/SplinesComponent.cpp          \
    Source/CurveEditor/Components/SelectionComponent.cpp        \
    Source/CurveEditor/Components/NavigationComponent.cpp       \
    Source/CurveEditor/Components/RulerComponent.cpp            \
    Source/CurveEditor/Tools/CurveEditorToolBase.cpp            \
    Source/CurveEditor/Tools/CurveEditorHoveringTool.cpp        \
    Source/CurveEditor/Tools/CurveEditorScrollTool.cpp          \
    Source/CurveEditor/Tools/CurveEditorZoomTool.cpp            \
    Source/CurveEditor/Tools/CurveEditorVisibleToolBase.cpp     \
    Source/CurveEditor/Tools/CurveEditorSelectionToolBase.cpp   \
    Source/CurveEditor/Tools/CurveEditorSelectionTool.cpp       \
    Source/CurveEditor/Tools/CurveEditorMovingTool.cpp          \
    Source/EditorWidgets/EditorViewWidget.cpp                   \

HEADERS += \
    Source/Pointers.h                                           \
    Source/MainWindow/MainWindow.h                              \
    Source/CurveEditor/ImGuiInterop.h                           \
    Source/CurveEditor/CurveEditorView.h                        \
    Source/CurveEditor/CurveEditorDataModel.h                   \
    Source/CurveEditor/CurveEditorController.h                  \
    Source/CurveEditor/CurveEditorListenerBase.h                \
    Source/CurveEditor/CurveEditorSplineListenerBase.h          \
    Source/CurveEditor/CurveEditorSelectionListenerBase.h       \
    Source/CurveEditor/CurveEditorFactory.h                     \
    Source/CurveEditor/CurveEditorSelectionDataModel.h          \
    Source/CurveEditor/CurveEditorSelectionController.h         \
    Source/CurveEditor/CurveEditorSelectionView.h               \
    Source/CurveEditor/SplineDataModel.h                        \
    Source/CurveEditor/KnotController.h                         \
    Source/CurveEditor/TangentController.h                      \
    Source/CurveEditor/CurveController.h                        \
    Source/CurveEditor/SplineController.h                       \
    Source/CurveEditor/SplineControllerFactory.h                \
    Source/CurveEditor/SplineFunctionController.h               \
    Source/CurveEditor/SplineView.h                             \
    Source/CurveEditor/SplineComponentControllerBase.h          \
    Source/CurveEditor/SplineComponentControllerBase.inl        \
    Source/CurveEditor/SplineComponentView.h                    \
    Source/CurveEditor/SplineComponentViewBase.h                \
    Source/CurveEditor/SplineComponentViewBase.inl              \
    Source/CurveEditor/SplineViewImplementation.h               \
    Source/CurveEditor/CurveEditorViewComponentBase.h           \
    Source/CurveEditor/CurveEditorViewComponentBase.inl         \
    Source/CurveEditor/CurveEditorViewVisibleComponentBase.h    \
    Source/CurveEditor/CurveEditorViewVisibleComponentBase.inl  \
    Source/CurveEditor/TangentViewComponent.h                   \
    Source/CurveEditor/KnotViewComponent.h                      \
    Source/CurveEditor/EditorContext.h                          \
    Source/CurveEditor/EditorRenderable.h                       \
    Source/CurveEditor/EditorRenderableBase.h                   \
    Source/CurveEditor/EditorView.h                             \
    Source/CurveEditor/EditorController.h                       \
    Source/CurveEditor/EditorControllerBase.h                   \
    Source/CurveEditor/EditorControllerBase.inl                 \
    Source/CurveEditor/EditorViewBase.h                         \
    Source/CurveEditor/EditorViewBase.inl                       \
    Source/CurveEditor/EditorListener.h                         \
    Source/CurveEditor/EditorListenableBase.h                   \
    Source/CurveEditor/EditorListenableBase.inl                 \
    Source/CurveEditor/EditorTool.h                             \
    Source/CurveEditor/CurveEditorTool.h                        \
    Source/CurveEditor/CurveEditorToolEvent.h                   \
    Source/CurveEditor/EditorDataModel.h                        \
    Source/CurveEditor/Utilities.h                              \
    Source/CurveEditor/Style.h                                  \
    Source/CurveEditor/Canvas.h                                 \
    Source/CurveEditor/Components/GridComponent.h               \
    Source/CurveEditor/Components/BackgroundComponent.h         \
    Source/CurveEditor/Components/DebugComponent.h              \
    Source/CurveEditor/Components/ToolHandlerComponent.h        \
    Source/CurveEditor/Components/SplinesComponent.h            \
    Source/CurveEditor/Components/SelectionComponent.h          \
    Source/CurveEditor/Components/NavigationComponent.h         \
    Source/CurveEditor/Components/RulerComponent.h              \
    Source/CurveEditor/Tools/CurveEditorToolBase.h              \
    Source/CurveEditor/Tools/CurveEditorHoveringTool.h          \
    Source/CurveEditor/Tools/CurveEditorScrollTool.h            \
    Source/CurveEditor/Tools/CurveEditorZoomTool.h              \
    Source/CurveEditor/Tools/CurveEditorVisibleToolBase.h       \
    Source/CurveEditor/Tools/CurveEditorSelectionToolBase.h     \
    Source/CurveEditor/Tools/CurveEditorSelectionTool.h         \
    Source/CurveEditor/Tools/CurveEditorMovingTool.h            \
    Source/EditorWidgets/EditorViewWidget.h

FORMS += \
    Source/MainWindow/UI/MainWindow.ui