CONFIG -= flat
TEMPLATE = app
TARGET = CurveEditor
BINDIR = $$OUT_PWD/bin
DESTDIR = $$BINDIR
QT += core gui widgets

include(Thirdparty/qtimgui/qtimgui.pri)

INCLUDEPATH += $$PWD/Thirdparty/ $$PWD/Thirdparty/qtimgui/imgui $$PWD/Source/CurveEditor $$PWD/Source

SOURCES += \
    Source/main.cpp \
    Source/MainWindow/MainWindow.cpp                    		\
    Source/CurveEditor/Editor.cpp                       		\
    Source/CurveEditor/EditorFactory.cpp                       		\
    Source/CurveEditor/EditorView.cpp                   		\
    Source/CurveEditor/Utilities.cpp                    		\
    Source/CurveEditor/Style.cpp                        		\
    Source/CurveEditor/Canvas.cpp                       		\
    Source/CurveEditor/Components/GridComponent.cpp	    		\
    Source/CurveEditor/Components/BackgroundComponent.cpp	    \
    Source/EditorWidgets/EditorViewWidget.cpp           		\
    
HEADERS += \
    Source/MainWindow/MainWindow.h                      		\
    Source/CurveEditor/ImGuiInterop.h                      		\
    Source/CurveEditor/Editor.h                         		\
    Source/CurveEditor/EditorFactory.h                         		\
    Source/CurveEditor/EditorView.h                     		\
    Source/CurveEditor/Utilities.h                      		\
    Source/CurveEditor/Style.h                          		\
    Source/CurveEditor/Canvas.h                         		\
    Source/CurveEditor/Components/GridComponent.h				\
    Source/CurveEditor/Components/BackgroundComponent.h	  		\
    Source/EditorWidgets/EditorViewWidget.h

FORMS += \
    Source/MainWindow/UI/MainWindow.ui