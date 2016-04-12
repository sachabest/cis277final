#include "point.h"
#include "math.h"

bool fequal2(float f1, float f2, float epsilon = 0.0001) {
    return fabs(f1 - f2) < epsilon;
}

Point::Point(float x, float y) : x(x), y(y) {

}

bool Point::operator<(const Point &p) const {
    // x major
    if (fequal2(this->x, p.x)) {
        return this->y < p.y;
    } else {
        return this->x < p.x;
    }
}
