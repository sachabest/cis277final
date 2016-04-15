#include "terrain.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include <iostream>

/**
 * @brief makeBounds - utility function to make bounding boxes
 * @param xmin - minimum x coordinate
 * @param ymin - minimum y coordinate
 * @param xmax - maximum x coordinate
 * @param ymax - maximum y coordinate
 * @return the created bounding box
 */
Bounds_t makeBounds(int xmin, int ymin, int xmax, int ymax) {
    Bounds_t bounds;
    bounds.xmin = xmin;
    bounds.ymin = ymin;
    bounds.xmax = xmax;
    bounds.ymax = ymax;
    return bounds;
}

/**
 * @brief shiftBounds - utility function used to move a bounding box
 * @param bounds - the box to move
 * @param dx - the delta x
 * @param dy - the delta y
 */
void shiftBounds(Bounds_t &bounds, int dx, int dy) {
    bounds.xmin += dx;
    bounds.xmax += dx;
    bounds.ymax += dy;
    bounds.ymin += dy;
}

Terrain::Terrain(int maxX, int maxY, int fequencyDivisor) {
    this->frequencyDivisor = fequencyDivisor;
    srand(time(NULL));
    for (int i = 0; i < maxX / fequencyDivisor; i++) {
        for (int j = 0; j < maxY / fequencyDivisor; j++) {
            createSeed(i, j);
        }
    }
    this->bounds = makeBounds(0, 0, maxY / frequencyDivisor, maxY / frequencyDivisor);
}

void Terrain::createSeed(int i, int j, bool checkExists) {
    Point p(i, j);
    if (checkExists && gradients.contains(p)) {
        return;
    }
    QVector<float> v;
    float random = ((float) rand()) / (float) RAND_MAX;
    v.push_back(random);
    random = ((float) rand()) / (float) RAND_MAX;
    v.push_back(random);
    gradients.insert(p, v);
}

void Terrain::removeSeed(int i, int j) {
    Point p(i, j);
    gradients.remove(p);
}

float Terrain::getBlock(float x, float y) {
    float unfloored_x = (x * (bounds.xmax - bounds.xmin)) + (bounds.xmin);
    float unfloored_y = (y * (bounds.ymax - bounds.ymin)) + (bounds.ymin);
    Point p(unfloored_x, unfloored_y);
    float height;
//    if (!heightmap.contains(p)) {
//        height = getHeight(x, y);
//        heightmap[p] = height;
//    } else {
//        height = heightmap[p];
//    }
    return getHeight(x, y);
}

/*
 * Use this funciton to create new terrain and destroy old terrain that is outside of the max range.
 * For example, we can shift +x/+y and dealloc the old blocks that were part of the original terrain
 * but not within the new range.
 *
 * (-x, +y)     (+x, +y)
 *        ______
 *        |    |
 *        |    |
 *        |____|
 *
 * (-x, -y)     (+x, -y)
 *
 */
void Terrain::shift(int idx, int idy) {
    int dx = idx / frequencyDivisor;
    int dy = idy / frequencyDivisor;

    // move bounds and create new seeds
    shiftBounds(this->bounds, dx, dy);

    for (int i = bounds.xmin; i <= bounds.xmax; i++) {
        for (int j = bounds.ymin; j <= bounds.ymax; j++) {
            createSeed(i, j, true);
        }
    }

    // clean up the heightmap cache
    for (Point p : heightmap.keys()) {
        if (p.x > bounds.xmax || p.x < bounds.xmin || p.y > bounds.ymax || p.y < bounds.ymin) {
            heightmap.remove(p);
        }
    }
}

/**
 * @brief lerp utility function
 * @param a0
 * @param a1
 * @param w
 * @return the lerped value
 */
float lerp(float a0, float a1, float w) {
    return (1.0 - w)*a0 + w*a1;
}

/**
 * @brief Terrain::dotGridGradient get the grid based dot product from the values (see algorithm)
 * @param x
 * @param y
 * @param dx
 * @param dy
 * @return the value of the dot at the location x, y
 */
float Terrain::dotGridGradient(int x, int y, float dx, float dy) {
    float rx = fabs(dx - x);
    float ry = fabs( dy - y);
    Point p = Point(x, y);

    return rx * gradients[p][0] + ry * gradients[p][1];
}

// https://en.wikipedia.org/wiki/Perlin_noise
// here the input floats are fractions we must find the closest sample for
float Terrain::getHeight(float x, float y) {
    // Determine grid cell coordinates
    // send to full grid
    float unfloored_x = (x * (bounds.xmax - bounds.xmin)) + (bounds.xmin);
    float unfloored_y = (y * (bounds.ymax - bounds.ymin)) + (bounds.ymin);

    int x0 = fmin(floor(unfloored_x), bounds.xmax - 1);
    int y0 = fmin(floor(unfloored_y), bounds.ymax - 1);
    int x1 = fmin(x0 + 1, bounds.xmax - 1);
    int y1 = fmin(y0 + 1, bounds.ymax - 1);
    x1 = fmax(x1, 0);
    y1 = fmax(y1, 0);

    float sx = unfloored_x - (float) x0;
    float sy = unfloored_y - (float) y0;

    float n0, n1, ix0, ix1, value;
    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = lerp(n0, n1, sx);
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = lerp(n0, n1, sx);
    value = lerp(ix0, ix1, sy);
    return value;
}
