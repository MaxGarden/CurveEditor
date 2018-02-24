CONFIG -= flat
TEMPLATE = app
TARGET = CurveEditor
BINDIR = $$OUT_PWD/bin
DESTDIR = $$BINDIR
QT += core gui widgets

include(Thirdparty/qtimgui/qtimgui.pri)

INCLUDEPATH += $$PWD/Thirdparty/qtimgui/ $$PWD/Thirdparty/qtimgui/imgui

SOURCES += \
    Source/main.cpp \
	Source/MainWindow/MainWindow.cpp
	
HEADERS += \
	Source/MainWindow/MainWindow.h

FORMS += \
	Source/MainWindow/UI/MainWindow.ui