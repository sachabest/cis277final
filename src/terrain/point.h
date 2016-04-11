#ifndef POINT_H
#define POINT_H

#include <QPoint>

class Point : public QPoint {

public:
    Point(int x, int y);
    bool operator<(const Point &p) const;
};

#endif // POINT_H
