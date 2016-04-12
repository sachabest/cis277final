#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "openGL/glwidget277.h"
#include "la.h"
#include <openGL/shaderprogram.h>
#include <scene/camera.h>
#include <scene/scene.h>
#include <scene/geometry/cube.h>
//#include <scene/geometry/chunk.h>
#include "la.h"
//#include "scene/user.h"
#include "scene/geometry/cross.h"
#include <QTimer>

class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:
    QOpenGLVertexArrayObject vao;

    ShaderProgram prog_lambert;
    ShaderProgram prog_flat;

    Camera gl_camera;//This is a camera we can move around the scene to view it from any angle.
    Cube geom_cube;
    //Chunk test_chunk;
    Scene scene;

    Cross cross;
    float gravity = 0.5;
    float terminal_v = 3.0;
    QTimer timer;
    bool leftx = false;
    bool rightx = false;
    bool upy = false;
    bool downy = false;
    bool inz = false;
    bool outz = false;
    bool do_nothing = false;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void SceneLoadDialog();
    void GLDrawScene();
    void ResizeToSceneCamera();

    void RaytraceScene();

    //to be called with mouse clicking events
    void destroyBlocks();
    void addBlocks();
    void collisionX(bool right);
    void collisionY(bool up);
    //if look is true we are trying to move towards us
    //is look is false we are trying to move away
    void collisionZ(bool look);

protected:
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);

signals:
    void sig_ResizeToCamera(int,int);

private slots:
    void timerUpdate();
};
