CONFIG -= flat
TEMPLATE = app
TARGET = CurveEditor
BINDIR = $$OUT_PWD/bin
DESTDIR = $$BINDIR
QT += core gui widgets

CONFIG += precompile_header suppress_vcproj_warnings
QMAKE_CXXFLAGS += /std:c++latest

PRECOMPILED_HEADER = Source/pch.h

include(Thirdparty/qtimgui/qtimgui.pri)

INCLUDEPATH += $$PWD/Thirdparty/ $$PWD/Thirdparty/qtimgui/imgui $$PWD/Source/CurveEditor $$PWD/Source

SOURCES += \
    Source/main.cpp \
    Source/MainWindow/MainWindow.cpp                    		\
    Source/CurveEditor/CurveEditorView.cpp                      \
    Source/CurveEditor/CurveEditorViewComponent.cpp             \
    Source/CurveEditor/CurveEditorDataModel.cpp                 \
    Source/CurveEditor/CurveEditorController.cpp                \
    Source/CurveEditor/CurveEditorFactory.cpp                  	\
	Source/CurveEditor/SplineDataModel.cpp						\
	Source/CurveEditor/SplineController.cpp						\
	Source/CurveEditor/SplineView.cpp							\
    Source/CurveEditor/EditorContext.cpp		            	\
    Source/CurveEditor/EditorView.cpp                   		\
    Source/CurveEditor/EditorController.cpp              		\
    Source/CurveEditor/EditorDataModel.cpp                		\
    Source/CurveEditor/Utilities.cpp                    		\
    Source/CurveEditor/Style.cpp                        		\
    Source/CurveEditor/Canvas.cpp                       		\
    Source/CurveEditor/Components/GridComponent.cpp	    		\
    Source/CurveEditor/Components/BackgroundComponent.cpp	    \
    Source/EditorWidgets/EditorViewWidget.cpp           		\
    
HEADERS += \
	Source/Pointers.h											\
    Source/MainWindow/MainWindow.h                      		\
    Source/CurveEditor/ImGuiInterop.h                      		\
    Source/CurveEditor/CurveEditorView.h                        \
    Source/CurveEditor/CurveEditorViewComponent.h               \
    Source/CurveEditor/CurveEditorDataModel.h   	            \
    Source/CurveEditor/CurveEditorController.h 	       	        \
    Source/CurveEditor/CurveEditorFactory.h               		\
	Source/CurveEditor/SplineDataModel.h						\
	Source/CurveEditor/SplineController.h						\
	Source/CurveEditor/SplineView.h							\
    Source/CurveEditor/EditorContext.h			            	\
    Source/CurveEditor/EditorView.h                     		\
    Source/CurveEditor/EditorController.h                 		\
    Source/CurveEditor/EditorDataModel.h                		\
    Source/CurveEditor/Utilities.h                      		\
    Source/CurveEditor/Style.h                          		\
    Source/CurveEditor/Canvas.h                         		\
    Source/CurveEditor/Components/GridComponent.h				\
    Source/CurveEditor/Components/BackgroundComponent.h	  		\
    Source/EditorWidgets/EditorViewWidget.h

FORMS += \
    Source/MainWindow/UI/MainWindow.ui