#ifndef TERRAIN_H
#define TERRAIN_H

#include "la.h"
#include "point.h"
#include <QMap>

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
private:
    Bounds_t bounds;
    QMap<Point, QVector<float>> gradients;
    QMap<Point, float> heightmap;

    void createSeed(int i, int j);
    void removeSeed(int i, int j);
    float getHeight(float x, float y);

    float dotGridGradient(int x, int y, float dx, float dy);
};

#endif // TERRAIN_H
