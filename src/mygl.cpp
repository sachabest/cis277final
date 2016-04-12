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
    test_chunk.create();

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    vao.bind();

    //Test scene data initialization


    cross.create();
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer.start(30);

    scene.CreateScene();
}

void MyGL::resizeGL(int w, int h)
{
    gl_camera = Camera(w, h, glm::vec3(scene.dimensions.x/2, scene.dimensions.y/2 + 2, scene.dimensions.z/2),
                       glm::vec3(scene.dimensions.x/2, scene.dimensions.y/2+2, scene.dimensions.z/2+1), glm::vec3(0,1,0));


    //camera is changed -> udpate user's perspective
    //user->cpos = gl_camera.eye;

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

    //draw the center of the gl lines
    glDisable(GL_DEPTH_TEST);
    //MODEL matrix is always identity so we are at the untransformed space of the cross
    prog_flat.setViewProjMatrix(glm::mat4(1.0f));
    prog_flat.setModelMatrix(glm::mat4(1.0f));
    prog_flat.draw(*this, cross);
    glEnable(GL_DEPTH_TEST);
}

void MyGL::GLDrawScene()
{
    for (Point3 p : scene.points) {
        prog_lambert.setModelMatrix(glm::translate(glm::mat4(), glm::vec3(p.x, p.y, p.z)));
        prog_lambert.draw(*this, geom_cube);
    }
}

//right is true -> move to right
//false-> try to move to left
void MyGL::collisionX(bool right) {
    glm::vec3 eye = gl_camera.eye;
    glm::vec3 xone = glm::vec3(eye.x-1, eye.y, eye.z);
    glm::vec3 xtwo = glm::vec3(eye.x+1, eye.y, cpos.z);
    glm::vec3 xthree = glm::vec3(eye.x-1, eye.y-1, eye.z);
    glm::vec3 xfour = glm::vec3(eye.x+1, eye.y-1, eye.z);

    //if all four blocks are empty -> you can move
    if (((!scene.objects[xone.x][xone.y][xone.z]) &&
            (!scene.objects[xthree.x][xthree.y][xthree.z]))
            || ((!scene.objects[xtwo.x][xtwo.y][xtwo.z])  && (!scene.objects[xfour.x][xfour.y][xfour.z]))) {
        //MOVEABLE
        //this case move to right
        if (right) {
            //but the camera is already being translated to the right????
        }
        else {

        }
    }

    //block; test which way it's blocking
    //else you don't move?????
    else {
        if (scene.objects[xone.x][xone.y][xone.z]) {

        }
        else if (scene.objects[xtwo.x][xtwo.y][xtwo.z]) {

        }
        else if (scene.objects[xthree.x][xthree.y][xthree.z]) {

        }
        else if (scene.objects[xfour.x][xfour.y][xfour.z]) {

        }
    }
}

//if up is true we are moving up;
//else we are trying to move down;
void MyGL::collisionY(bool up) {
    glm::vec3 eye = gl_camera.eye;
    glm::vec3 yone = glm::vec3(eye.x, eye.y+1, eye.z);
    glm::vec3 ytwo = glm::vec3(eye.x, eye.y-2, eye.z);

    //you can move up and you are trying to move up
    if (up && !scene.objects[yone.x][yone.y][yone.z]) {

    }
    //you can move down; gravity and you are trying to move down
    else if (!up && !scene.objects[ytwo.x][ytwo.y][ytwo.z]) {

    }

}

//if look is true we are trying to move towards us
//is look is false we are trying to move away
void MyGL::collisionZ(bool look) {
    glm::vec3 eye = gl_camera.eye;

    glm::vec3 zone = glm::vec3(eye.x, eye.y, eye.z-1);
    glm::vec3 ztwo = glm::vec3(eye.x, eye.y, eye.z+1);
    glm::vec3 zthree = glm::vec3(eye.x, eye.y-1, eye.z-1);
    glm::vec3 zfour = glm::vec3(eye.x, eye.y+1, eye.z+1);

    //if all four blocks are empty -> you can move
    if (((!scene.objects[zone.x][zone.y][zone.z]) &&
            (!scene.objects[zthree.x][zthree.y][zthree.z]))
            || ((!scene.objects[ztwo.x][ztwo.y][ztwo.z])  && (!scene.objects[zfour.x][zour.y][zfour.z]))) {
        //MOVEABLE
        //this case move to the front
        if (look) {
            //but the camera is already being translated to the right????
        }
        else {

        }
    }
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
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
    }
    //z direction
    else if (e->key() == Qt::Key_W) {
        gl_camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        gl_camera.TranslateAlongLook(-amount);
    }
    //x direction
    else if (e->key() == Qt::Key_D) {
        gl_camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        gl_camera.TranslateAlongRight(-amount);
    }

    //y direction
    else if (e->key() == Qt::Key_Q) {
        gl_camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        gl_camera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_P) {
        // temp
        scene.shift(16, 0, 16);
    }
    gl_camera.RecomputeAttributes();

    //camera is changed; have to update user's perspective
    //user->cpos = gl_camera.eye;
    //CALL user.collision ON EVERY CUBE IN THIS WORLD
    //user->collision();

    update();  // Calls paintGL, among other things
}

//screen center (0,0,0)
void MyGL::destroyBlocks() {
    std::cout << "destroy blocks" << std::endl;
    Ray ray_from_center = gl_camera.raycast();
    //for all the cubes in the world
    //if the intersect return true
    //get rid of them
    QList<QList<QList<bool>>> scene_objs = scene.objects;

//    for (int i = 0; i < scene.dimensions.x; i++) {
//        for (int j = 0; j < scene.dimensions.y; j++) {
//            for (int k = 0; k < scene.dimensions.z; k++) {
//                //if there is geoemtry here
//                if (scene_objs[i][j][k]) {
//                    //test for intersect
//                    //call cube.intersect
//                    //if true, delete
//                }
//            }
//        }
//    }

    //RAY MARCH from 1 to 31 (< 32 taxicabs)
    for (int t = 1; t < 32; t++) {
        glm::vec3 new_dir = glm::vec3 (t*ray_from_center.direction.x, t*ray_from_center.direction.y,
                                       t*ray_from_center.direction.z);
        glm::vec3 position = ray_from_center.origin + new_dir;
        //floor the position value and check if there is an object in there;
        //if there is: remove it and break out of the loop
        if (scene_objs[glm::floor(position.x)][glm::floor(position.y)][glm::floor(position.z)])
        {
            //DESTROY THE CUBE
            //CAROLINA'S STORAGE
        }
    }
}

void MyGL::addBlocks() {
    std::cout << "add block" << std::endl;
    //RAYMARCH THIS
    Ray ray_from_center = gl_camera.raycast();
    QList<QList<QList<bool>>> scene_objs = scene.objects;

    //RAY MARCH from 1 to 31 (< 32 taxicabs)
    for (int t = 1; t < 32; t++) {
        glm::vec3 new_dir = glm::vec3 (t*ray_from_center.direction.x, t*ray_from_center.direction.y,
                                       t*ray_from_center.direction.z);
        glm::vec3 position = ray_from_center.origin + new_dir;
        //floor the position value and check if there is an object in there;
        //if there is: remove it and break out of the loop
        if (scene_objs[glm::floor(position.x)][glm::floor(position.y)][glm::floor(position.z)])
        {
            //we found a cube; find its face normals; dot it with ray_from_center.direction
            //whichever one returns a value < 0 -> add a cube next to it (NOT really sure how??)
        }
    }
}

//MOUSE clicking destroying and adding blocks
void MyGL::mousePressEvent(QMouseEvent *e) {
    //check for destruction of cubes
    if (e->button() == Qt::LeftButton) {
        destroyBlocks();
    }
    //check for adding things
    else if (e->button() == Qt::RightButton) {
        addBlocks();
    }
    update();//calls painGL
}

void MyGL::timerUpdate()
{
    // This function is called roughly 60 times per second.
    // Use it to update your scene and then tell it to redraw.
    // (Don't update your scene in paintGL, because it
    // sometimes gets called automatically by Qt.)

    update();
}
