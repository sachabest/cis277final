#ifndef POINT3_H
#define POINT3_H

#include <la.h>
#include "ray.h"
#include "transform.h"

class Point3 {
public:
    Point3();
    Point3(float x, float y, float z);
    float x;
    float y;
    float z;
    bool operator<(const Point3 &p) const;
    bool operator==(const Point3 &p) const;
    Point3 operator-(const Point3 &p) const;
    glm::vec3 toVec3();

    //check for floating point errors with comparisons
    bool nearlyEqual(float a, float b);
    //CHECK FOR IF THE RAY INTERSECTS WITH THIS CUBE
    glm::vec3 intersect(Ray r, Transform cube_to_world);
};

#endif // POINT3_H
