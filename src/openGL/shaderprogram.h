#pragma once

#include <openGL/drawable.h>
#include <openGL/glwidget277.h>
#include <la.h>

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <iostream>


class ShaderProgram
{
public:
    QOpenGLShaderProgram prog;

    int attrPos;
    int attrNor;
    int attrCol;
    int attrUV;

    int unifModel;
    int unifModelInvTr;
    int unifViewProj;
    int unifColor;
    int unifUV;

    QOpenGLTexture* textSampler;

public:
    void create(const char *vertfile, const char *fragfile);
    void setModelMatrix(const glm::mat4 &model);
    void setViewProjMatrix(const glm::mat4& vp);
    void setUVImage(QOpenGLTexture* texture);
    void draw(GLWidget277 &f, Drawable &d);
};
