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
    Terrain(int maxX, int maxY);
    float getHeight(float x, float y);
    void shift(int dx, int dy);
private:
    Bounds_t bounds;
    QMap<Point, QVector<float>> gradients;

    void createSeed(int i, int j);
    void removeSeed(int i, int j);
    float dotGridGradient(int x, int y, float dx, float dy);
};

#endif // TERRAIN_H
