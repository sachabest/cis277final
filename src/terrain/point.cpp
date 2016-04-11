#include "point.h"

Point::Point(float x, float y) : x(x), y(y) {

}

bool Point::operator<(const Point &p) const {
    // x major
    if (this->x == p.x) {
        return this->y < p.y;
    } else {
        return this->x < p.x;
    }
}
