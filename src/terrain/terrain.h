#ifndef TERRAIN_H
#define TERRAIN_H

#include "la.h"
#include "point.h"
#include <QMap>
#include <scene/geometry/chunk.h>
#include <scene/point3.h>

struct Bounds_t {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
};

class Terrain {
public:
    Terrain(int maxX, int maxY, int frequenceDivisor = 8);
    void shift(int dx, int dy);
    float getBlock(float x, float y);
    QMap<Point3, Chunk*> chunk_map;
private:
    Bounds_t bounds;
    QMap<Point, QVector<float>> gradients;
    QMap<Point, float> heightmap;
    int frequencyDivisor;
    void createSeed(int i, int j, bool checkExists = false);
    void removeSeed(int i, int j);
    float getHeight(float x, float y);

    float dotGridGradient(int x, int y, float dx, float dy);
};

#endif // TERRAIN_H
