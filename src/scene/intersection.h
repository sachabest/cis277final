#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "point3.h"

class Intersection
{
public:
    Intersection();
    Intersection(Point3 point, float t_val);
    Point3 point_val;
    float t;
};

#endif // INTERSECTION_H
