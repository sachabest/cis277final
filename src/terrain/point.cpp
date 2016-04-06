#include "point.h"

Point::Point(int x, int y) : QPoint(x, y) {

}

bool Point::operator <(Point p1, Point p2) {
    // x major
    if (p1.x() == p2.x()) {
        return p1.y() < p2.y();
    } else {
        return p1.x() < p2.x();
    }
}
