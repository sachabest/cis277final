#pragma once
#include <QList>
#include <scene/camera.h>
#include "terrain/terrain.h"
#include "point3.h"

class Scene {
public:
    Scene();

    void CreateTestScene();

    QList<Point3> points;
    glm::ivec3 dimensions;
    Terrain terrain;
};
