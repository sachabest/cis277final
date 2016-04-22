QT += core widgets

TARGET = 277
TEMPLATE = app
CONFIG += c++11
CONFIG += warn_on
CONFIG += debug

INCLUDEPATH += include

include(src/src.pri)

FORMS += forms/mainwindow.ui \
    forms/cameracontrolshelp.ui \
    forms/hud.ui

RESOURCES += glsl.qrc

HEADERS += \
    forms/hud.h

SOURCES += \
    forms/hud.cpp
