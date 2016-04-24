#ifndef TERRAIN_H
#define TERRAIN_H

#include "la.h"
#include "point.h"
#include <QMap>
#include <scene/geometry/chunk.h>
#include <scene/point3.h>
#include <scene/octnode.h>
#include <QImage>
#include <QOpenGLTexture>

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
    void setHeight(float x, float y, float height);
private:
    Bounds_t bounds;
    QMap<Point, QVector<float>> gradients;
    int frequencyDivisor;
    void createSeed(int i, int j, bool checkExists = false);
    void removeSeed(int i, int j);
    float getHeight(float x, float y);

    float dotGridGradient(int x, int y, float dx, float dy);
    QMap<Point, float> heightmap;
};

#endif // TERRAIN_H
