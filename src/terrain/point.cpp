#include "point.h"

Point::Point(int x, int y) : QPoint(x, y) {

}

bool Point::operator<(const Point &p) const {
    // x major
    if (this->x() == p.x()) {
        return this->y() < p.y();
    } else {
        return this->x() < p.x();
    }
}
