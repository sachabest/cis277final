#pragma once
#include <QList>
#include <scene/camera.h>
#include "terrain/terrain.h"
#include "point3.h"
#include <scene/geometry/chunk.h>
#include "generators/lparser.h"

class Scene {
public:
    Scene();

    void CreateScene();
    void CreateChunkScene();
    void CreateNewChunks();
    void findNearbyChunks();
    void shift(int dx, int dy, int dz);

    QList<Point3> voxelize(const QVector<LPair_t> &pairs);
    QList<Point3> points;
    glm::ivec3 dimensions;
    glm::vec3 origin;
    Terrain terrain;

    int num_chunks;
    QList<Point3> chunk_points;
};
