#ifndef POINT_H
#define POINT_H

class Point {

public:
    Point(float x, float y);
    bool operator<(const Point &p) const;
    float x;
    float y;
};

#endif // POINT_H
