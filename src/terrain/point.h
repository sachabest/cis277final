#ifndef POINT_H
#define POINT_H

#include <QPoint>

class Point : public QPoint {

    Q_OBJECT

public:
    Point(int x, int y);
    bool operator<(Point p1, Point p2);
};

#endif // POINT_H
