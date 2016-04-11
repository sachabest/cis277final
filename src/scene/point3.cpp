#include "point3.h"
#include "math.h"

bool fequal(float f1, float f2, float epsilon = 0.0001) {
    return fabs(f1 - f2) < epsilon;
}

Point3::Point3(float x, float y, float z) : x(x), y(y), z(z) {}

bool Point3::operator<(const Point3 &p) const {
    // x major
    if (fequal(this->x, p.x)) {
        if (fequal(this->y, p.y)) {
            return (fequal(this->z, p.z));
        } else {
            return this->y < p.y;
        }
    } else {
        return this->x < p.x;
    }
}
