#include "point3.h"
#include "math.h"
#include <QDebug>

bool fequal(float f1, float f2, float epsilon = 0.001) {
    return fabs(f1 - f2) < epsilon;
}

Point3::Point3(float x, float y, float z) : x(x), y(y), z(z) {}

bool Point3::operator<(const Point3 &p) const {
    // x major
    if (fequal(this->x, p.x)) {
        if (fequal(this->y, p.y)) {
            // return (fequal(this->z, p.z));
            return this->z < p.z;
        } else {
            return this->y < p.y;
        }
    } else {
        return this->x < p.x;
    }
}

bool Point3::operator==(const Point3 &p) const {
    return fequal(this->x, p.x) && fequal(this->y, p.y) && fequal(this->z, p.z);
}

Point3 Point3::operator-(const Point3 &p) const {
    return Point3(this->x - p.x, this->y - p.y, this->z - p.z);
}

glm::vec3 Point3::toVec3() {
    return glm::vec3(this->x, this->y, this->z);
}
