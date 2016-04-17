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
    //test_chunk.create();

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    vao.bind();

    //Test scene data initialization


    cross.create();
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer.start(100);

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
    Point3 xone = Point3(glm::floor(eye.x-1), glm::floor(eye.y), glm::floor(eye.z));
    Point3 xtwo = Point3(glm::floor(eye.x+1), glm::floor(eye.y), glm::floor(eye.z));
    Point3 xthree = Point3(glm::floor(eye.x-1), glm::floor(eye.y-1), glm::floor(eye.z));
    Point3 xfour = Point3(glm::floor(eye.x+1), glm::floor(eye.y-1), glm::floor(eye.z));

    QList<Point3> points = scene.points;

    if (!right) {
        if (!(points.contains(xone)) && !(points.contains(xthree))) {
            std::cout << "in trying to move left" << std::endl;
            leftx = true;
            for (int i = 0 ; i < 3; i++) {
                //float amount = gravity * i;
                float amount = 1.0f;
                gl_camera.TranslateAlongRight(-amount);
            }
            //leftx = false;
        }
    }
    else if (right) {
        if (!(points.contains(xtwo)) && !(points.contains(xfour))) {
            std::cout << "in tryin to move right" << std::endl;
            rightx = true;
            for (int i = 0 ; i < 3; i++) {
                //float amount = gravity * i;
                float amount = 1.0f;
                gl_camera.TranslateAlongRight(amount);
            }
            //rightx = false;
        }
    }

    //if all four blocks are empty -> you can move
 /*   if (((!scene.objects[xone.x][xone.y][xone.z]) &&
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
    } */
}

//if up is true we are moving up;
//else we are trying to move down;
void MyGL::collisionY(bool up) {
    //std::cout << "here in collision y" << std::endl;
    glm::vec3 eye = gl_camera.eye;
    Point3 yone = Point3(glm::floor(eye.x), glm::floor(eye.y+0.5), glm::floor(eye.z));
    Point3 ytwo = Point3(glm::floor(eye.x), glm::floor(eye.y-3), glm::floor(eye.z));

    QList<Point3> points = scene.points;
    //you can move up and you are trying to move up
 /*   if (up && !scene.objects[yone.x][yone.y][yone.z]) {

    }
    //you can move down; gravity and you are trying to move down
    else if (!up && !scene.objects[ytwo.x][ytwo.y][ytwo.z]) {

    }*/

    if (!up) {
        if (!points.contains(ytwo)) {
            for (int i = 0 ; i < 3; i++) {
                float amount = gravity * i * i;
                gl_camera.TranslateAlongUp(-amount);
                //do_nothing = true;
            }
        }
    }
    else if (up) {
        if (!points.contains(yone)) {
            for (int i = 0 ; i < 3; i++) {
                //terminal v is 4;
                float amount = gravity * i * i;
                gl_camera.TranslateAlongUp(amount);
            }
        }
        //downy = true;
    }
}

//if look is true we are trying to move towards us
//is look is false we are trying to move away
void MyGL::collisionZ(bool look) {
    glm::vec3 eye = gl_camera.eye;

    Point3 zone = Point3(glm::floor(eye.x), glm::floor(eye.y), glm::floor(eye.z-1));
    Point3 ztwo = Point3(glm::floor(eye.x), glm::floor(eye.y), glm::floor(eye.z+1));
    Point3 zthree = Point3(glm::floor(eye.x), glm::floor(eye.y-1), glm::floor(eye.z-1));
    Point3 zfour = Point3(glm::floor(eye.x), glm::floor(eye.y+1), glm::floor(eye.z+1));


    QList<Point3> points = scene.points;

    if (!look) {
        if (!(points.contains(zone)) && !(points.contains(zthree))) {
            std::cout << "moving out" << std::endl;
            outz = true;
            for (int i = 0 ; i < 3; i++) {
                //float amount = i*gravity;
                float amount = 1.0f;
                gl_camera.TranslateAlongLook(-amount);
            }
            //outz = false;
        }
    }

    else if(look) {
        if (!(points.contains(ztwo)) && !(points.contains(zfour))) {
            std::cout << "moving in" << std::endl;
            inz = true;

            for (int i = 0 ; i < 3; i++) {
                //float amount = gravity * i;
                float amount = 1.0f;
                gl_camera.TranslateAlongLook(amount);
            }
            //inz = false;
        }
    }
    //if all four blocks are empty -> you can move
   /* if (((!scene.objects[zone.x][zone.y][zone.z]) &&
            (!scene.objects[zthree.x][zthree.y][zthree.z]))
            || ((!scene.objects[ztwo.x][ztwo.y][ztwo.z])  && (!scene.objects[zfour.x][zfour.y][zfour.z]))) {
        //MOVEABLE
        //this case move to the front
        if (look) {
            //but the camera is already being translated to the right????
        }
        else {

        }
    }*/
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
        //gl_camera.TranslateAlongLook(amount);
        inz = true;
        timerUpdate();
        inz = false;

    } else if (e->key() == Qt::Key_S) {
        //gl_camera.TranslateAlongLook(-amount);
        outz = true;
        timerUpdate();
        outz = false;
    }
    //x direction
    else if (e->key() == Qt::Key_D) {
 //       gl_camera.TranslateAlongRight(amount);
        rightx = true;
        timerUpdate();
        rightx = false;

    } else if (e->key() == Qt::Key_A) {
       // gl_camera.TranslateAlongRight(-amount);
        leftx = true;
        timerUpdate();
        leftx = false;
    }

    //y direction
    else if (e->key() == Qt::Key_Q) {
        //gl_camera.TranslateAlongUp(-amount);
        downy = true;
        timerUpdate();
        downy = false;
    } else if (e->key() == Qt::Key_E) {
        //gl_camera.TranslateAlongUp(amount);
        upy = true;
        timerUpdate();
        upy = false;
    }

    else if (e->key() == Qt::Key_P) {
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

    QList<Point3> points = scene.points;
    std::cout << "points size " << scene.points.size() << std::endl;

    //RAY MARCH from 1 to 31 (< 32 taxicabs)
    for (int t = 1; t < 32; t++) {
        glm::vec3 new_dir = glm::vec3 (t*ray_from_center.direction.x, t*ray_from_center.direction.y,
                                       t*ray_from_center.direction.z);
        glm::vec3 position = ray_from_center.origin + new_dir;
        //floor the position value and check if there is an object in there;
        //if there is: remove it and break out of the loop
        Point3 point_cube = Point3(glm::floor(position.x), glm::floor(position.y), glm::floor(position.z));
        if (points.contains(point_cube))
        {
            std::cout << "in destroying" << std::endl;
            for (int i = 0; i < points.size(); i++) {
                if(points[i] == point_cube) {
                    std::cout << "remove stuff" << std::endl;
                    scene.points.removeAt(i);
                    std::cout << "after remove point size " << scene.points.size() << std::endl;
                    break;
                }
            }
        }
        update();
    }
}

void MyGL::addBlocks() {
    std::cout << "add block" << std::endl;
    //RAYMARCH THIS
    Ray ray_from_center = gl_camera.raycast();
    //QList<QList<QList<bool>>> scene_objs = scene.objects;
    //QList<Point3>& points = scene.points;

    QList<Point3> points = scene.points;
    std::cout << "points size " << scene.points.size() << std::endl;

    //RAY MARCH from 1 to 31 (< 32 taxicabs)
    for (int t = 1; t < 32; t++) {
        glm::vec3 new_dir = glm::vec3 (t*ray_from_center.direction.x, t*ray_from_center.direction.y,
                                       t*ray_from_center.direction.z);
        glm::vec3 position = ray_from_center.origin + new_dir;
        Point3 point_cube = Point3(glm::floor(position.x), glm::floor(position.y), glm::floor(position.z));
        Ray newRay = Ray(position, ray_from_center.direction);
        if (points.contains(point_cube))
        {
            std::cout << "in adding" << std::endl;
            glm::vec3 T = position;
            glm::vec3 R = glm::vec3(0,0,0);
            glm::vec3 S = glm::vec3(1,1,1);

            for (int i = 0; i < points.size(); i++) {
                if(points[i] == point_cube) {
                    std::cout << "add stuff" << std::endl;

                    glm::vec3 normal = points[i].intersect(newRay, Transform(T, R, S));
                    std::cout << "normal x " << normal.x << std::endl;
                    std::cout << "normal y " << normal.y << std::endl;
                    std::cout << "normal z " << normal.z << std::endl;

                    if (normal == glm::vec3(-1,0,0)) {
                        //negative x; put one on right
                        Point3 potential = Point3(point_cube.x-1, point_cube.y, point_cube.z);
                        if (!points.contains(potential)) {
                            scene.points.append(potential);
                            break;
                        }
                    }
                    else if (normal == glm::vec3(1,0,0)) {
                        Point3 potential = Point3(point_cube.x+1, point_cube.y, point_cube.z);
                        if (!points.contains(potential)) {
                            scene.points.append(potential);
                            break;
                        }
                    }
                    else if (normal == glm::vec3(0,-1,0)) {
                        Point3 potential = Point3(point_cube.x, point_cube.y-1, point_cube.z);
                        if (!points.contains(potential)) {
                            scene.points.append(potential);
                            break;
                        }
                    }
                    else if (normal == glm::vec3(0,1,0)) {
                        Point3 potential = Point3(point_cube.x, point_cube.y+1, point_cube.z);
                        if (!points.contains(potential)) {
                            scene.points.append(potential);
                            break;
                        }
                    }
                    else if (normal == glm::vec3(0,0,-1)) {
                        Point3 potential = Point3(point_cube.x, point_cube.y, point_cube.z-1);
                        if (!points.contains(potential)) {
                            scene.points.append(potential);
                            break;
                        }
                    }
                    else if (normal == glm::vec3(0,0,1)) {
                        Point3 potential = Point3(point_cube.x, point_cube.y, point_cube.z+1);
                        if (!points.contains(potential)) {
                            scene.points.append(potential);
                            break;
                        }
                    }
                    else{
                        //dont do anything
                    }

                    //scene.points.removeAt(i);
                    std::cout << "after add point size " << scene.points.size() << std::endl;
                    break;
                }
            }

        }
        update();
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

    //trying to move left
    if (leftx) {
        collisionX(false);
    }
    else if (rightx) {
        collisionX(true);
    }
    else if (upy) {
        collisionY(true);
    }
    else if (downy) {
        collisionY(false);
    }
    else if (outz) {
        //moving away
        collisionZ(false);
    }
    else if (inz) {
        collisionZ(true);
    }
    //we are just starting
    else if (do_nothing) {

    }
    else {
        collisionY(false);
    }

    update();
}
