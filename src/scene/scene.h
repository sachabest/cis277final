#pragma once
#include <QList>
#include <scene/camera.h>
#include "terrain/terrain.h"
#include "point3.h"

class Scene {
public:
    Scene();

    void CreateScene();
    void shift(int dx, int dy, int dz);

    QList<Point3> points;
    glm::ivec3 dimensions;
    glm::vec3 origin;
    Terrain terrain;


};
