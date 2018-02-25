CONFIG -= flat
TEMPLATE = app
TARGET = CurveEditor
BINDIR = $$OUT_PWD/bin
DESTDIR = $$BINDIR
QT += core gui widgets

include(Thirdparty/qtimgui/qtimgui.pri)

INCLUDEPATH += $$PWD/Thirdparty/qtimgui/ $$PWD/Thirdparty/qtimgui/imgui $$PWD/Source/CurveEditor $$PWD/Source

SOURCES += \
    Source/main.cpp \
	Source/MainWindow/MainWindow.cpp 					\
	Source/CurveEditor/Editor.cpp 						\
	Source/CurveEditor/EditorView.cpp 					\
	Source/EditorWidgets/EditorViewWidget.cpp			\
	
HEADERS += \
	Source/MainWindow/MainWindow.h 						\
	Source/CurveEditor/Editor.h		 					\
	Source/CurveEditor/EditorView.h 					\
	Source/EditorWidgets/EditorViewWidget.h

FORMS += \
	Source/MainWindow/UI/MainWindow.ui