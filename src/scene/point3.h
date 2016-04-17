#ifndef POINT3_H
#define POINT3_H

#include <la.h>

class Point3 {
public:
    Point3(float x, float y, float z);
    float x;
    float y;
    float z;
    bool operator<(const Point3 &p) const;
    bool operator==(const Point3 &p) const;
    Point3 operator-(const Point3 &p) const;
    glm::vec3 toVec3();
};

#endif // POINT3_H
