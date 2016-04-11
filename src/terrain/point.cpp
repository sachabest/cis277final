#include "point.h"

Point::Point(int x, int y) : QPoint(x, y) {

}

//<<<<<<< HEAD
//bool Point::operator<(const Point& p1, const Point& p2) {
//=======
bool Point::operator<(const Point &p) const {
//>>>>>>> upstream/master
    // x major
    if (this->x() == p.x()) {
        return this->y() < p.y();
    } else {
        return this->x() < p.x();
    }
}
