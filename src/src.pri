INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
#LIBS += -L$$PWD/lib -ltbb

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/mygl.cpp \
    $$PWD/scene/camera.cpp \
    $$PWD/scene/scene.cpp \
    $$PWD/scene/geometry/cube.cpp \
    $$PWD/openGL/drawable.cpp \
    $$PWD/openGL/glwidget277.cpp \
    $$PWD/openGL/shaderprogram.cpp \
    $$PWD/scene/transform.cpp \
    $$PWD/cameracontrolshelp.cpp \
    $$PWD/terrain/terrain.cpp \
    $$PWD/terrain/point.cpp \
    $$PWD/scene/user.cpp \
    $$PWD/scene/ray.cpp \
    $$PWD/scene/geometry/cross.cpp

HEADERS += \
    $$PWD/mainwindow.h \
    $$PWD/mygl.h \
    $$PWD/scene/camera.h \
    $$PWD/la.h \
    $$PWD/drawable.h \
    $$PWD/scene/scene.h \
    $$PWD/scene/geometry/cube.h \
    $$PWD/openGL/drawable.h \
    $$PWD/openGL/glwidget277.h \
    $$PWD/openGL/shaderprogram.h \
    $$PWD/scene/transform.h \
    $$PWD/scene/materials/material.h \
    $$PWD/raytracing/film.h \
    $$PWD/raytracing/integrator.h \
    $$PWD/cameracontrolshelp.h \
    $$PWD/scene/terrain.h \
    $$PWD/terrain/terrain.h \
    $$PWD/terrain/point.h \
    $$PWD/scene/user.h \
    $$PWD/scene/ray.h \
    $$PWD/scene/geometry/cross.h
