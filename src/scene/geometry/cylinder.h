#pragma once

#include <opengl/drawable.h>
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Cylinder : public Drawable
{
public:
    Cylinder();
    virtual void create();
};
