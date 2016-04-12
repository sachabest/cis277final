#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QXmlStreamReader>
#include <QFileDialog>
#include <QTime>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent)
{
    setFocusPolicy(Qt::ClickFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    vao.destroy();
    for (Chunk* chunk : scene.terrain.chunk_map.values()) {
        delete chunk;
    }
}

void MyGL::initializeGL()
{
    // Create an OpenGL context
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    vao.create();

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat-color shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    geom_cube.create();
    //test_chunk.create();

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    vao.bind();

    //Test scene data initialization
    scene.CreateScene();
    scene.CreateChunkScene();
}

void MyGL::resizeGL(int w, int h)
{
    //    gl_camera = Camera(w, h, glm::vec3(scene.dimensions.x/2, scene.dimensions.y/2 + 2, scene.dimensions.z/2),
    //                       glm::vec3(scene.dimensions.x/2, scene.dimensions.y/2+2, scene.dimensions.z/2+1), glm::vec3(0,1,0));

    gl_camera = Camera(w, h, glm::vec3(0, 10, 0),
                       glm::vec3(10, 2, 10), glm::vec3(0,1,0));

    glm::mat4 viewproj = gl_camera.getViewProj();

    // Upload the projection matrix
    prog_lambert.setViewProjMatrix(viewproj);
    prog_flat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

// This function is called by Qt any time your GL window is supposed to update
// For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the viewproj matrix
    prog_lambert.setViewProjMatrix(gl_camera.getViewProj());
    prog_flat.setViewProjMatrix(gl_camera.getViewProj());
    GLDrawScene();
}

void MyGL::GLDrawScene()
{
    //    for (Point3 p : scene.points) {
    //        prog_lambert.setModelMatrix(glm::translate(glm::mat4(), glm::vec3(p.x, p.y, p.z)));
    //        prog_lambert.draw(*this, geom_cube);
    //        }

    int num_chunks = scene.num_chunks;
    for (int x = 0; x < num_chunks; x++) {
        for (int z = 0; z < num_chunks; z++) {
            prog_lambert.setModelMatrix(glm::translate(glm::mat4(), glm::vec3((x-(num_chunks/2))*16 + scene.origin.x, 0, (z-(num_chunks/2))*16 + scene.origin.z)));
            prog_lambert.draw(*this, *(scene.terrain.chunk_map.value(Point3((x-(num_chunks/2))*16.0f + scene.origin.x, 0, (z-(num_chunks/2))*16.0f + scene.origin.z))));
        }
    }

}

// Given the current camera position, which chunk am I located on?
Point3 MyGL::getChunkPosition()
{
    return Point3(glm::floor(gl_camera.eye.x/16.0f), 0, glm::floor(gl_camera.eye.y/16.0f));
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    Point3 old_pos = getChunkPosition();
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        gl_camera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        gl_camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        gl_camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        gl_camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        gl_camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        gl_camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        gl_camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        gl_camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        gl_camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        gl_camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        gl_camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        gl_camera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_P) {
        // temp
        scene.shift(16, 0, 16);
    }
    gl_camera.RecomputeAttributes();
    Point3 new_pos = getChunkPosition();
    // We moved to a different chunk
    if (!(old_pos == new_pos)) {
        qDebug() << "Switched chunks";
//        if (old_pos.x < new_pos.x) {
//            scene.shift(-16, 0, 0);
//        } else if (old_pos.x > new_pos.x) {
//            scene.shift(16, 0, 0);
//        } else if (old_pos.z < new_pos.z) {
//            scene.shift(0, 0, -16);
//        } else if (old_pos.z > new_pos.z) {
//            scene.shift(0, 0, 16);
//        }
    }
    update();  // Calls paintGL, among other things
}
