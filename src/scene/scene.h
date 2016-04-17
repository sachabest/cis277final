#pragma once
#include <QList>
#include <scene/camera.h>
#include "terrain/terrain.h"
#include "point3.h"
#include <scene/geometry/chunk.h>
#include "generators/lparser.h"
#include <QOpenGLTexture>


class Scene {

    static const int tree_length = 64;

public:
    Scene();

    QOpenGLTexture* texture;
    void CreateScene();
    void CreateChunkScene();
    void CreateNewChunks();
    void findNearbyChunks();
    void shift(int dx, int dy, int dz);

    Chunk* getContainingChunk(Point3 p) const;
    OctNode* getContainingNode(Point3 p) const;
    Point3 worldToChunk(Point3 p);
    void voxelize(const QVector<LPair_t> &pairs);
    void bresenham(const glm::vec4 &p1, const glm::vec4 &p2);

    bool isFilled(Point3 p);
    glm::ivec3 dimensions;
    glm::vec3 origin;
    Terrain terrain;
    OctNode* octree;

    int num_chunks;
    QList<Point3> chunk_points;
private:
    void addVoxel(QSet<OctNode *> &set, Point3 &p);
};
