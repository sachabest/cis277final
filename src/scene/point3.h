#ifndef POINT3_H
#define POINT3_H

class Point3 {
public:
    Point3(float x, float y, float z);
    float x;
    float y;
    float z;
    bool operator<(const Point3 &p) const;
    bool operator==(const Point3 &p) const;
};

#endif // POINT3_H
