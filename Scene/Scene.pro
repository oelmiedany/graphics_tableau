TEMPLATE = app
TARGET = Scene
INCLUDEPATH += . /opt/local/include

QT += widgets opengl gui

LIBS += -lGLU

HEADERS += \
    SceneWidget.h \
    SceneWindow.h

SOURCES += \
    SceneMain.cpp \
    SceneWidget.cpp \
    SceneWindow.cpp
