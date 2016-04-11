#ifndef POINT_H
#define POINT_H

#include <QPoint>
#include <iostream>

class Point : public QPoint {
    Q_OBJECT

public:
    Point(int x, int y);
    bool operator<(this, const Point& p1, const Point& p2);
};

#endif // POINT_H
