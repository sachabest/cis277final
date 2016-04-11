#include "point.h"

Point::Point(int x, int y) : QPoint(x, y) {

}

bool Point::operator<(const Point& p1, const Point& p2) {
    // x major
    if (p1.x() == p2.x()) {
        return p1.y() < p2.y();
    } else {
        return p1.x() < p2.x();
    }
}
