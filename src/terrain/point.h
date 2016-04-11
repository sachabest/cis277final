#ifndef POINT_H
#define POINT_H

#include <QPoint>
#include <iostream>

class Point : public QPoint {
//<<<<<<< HEAD
    Q_OBJECT

//public:
//    Point(int x, int y);
//    bool operator<(const Point& p1, const Point& p2);
//=======

public:
    Point(int x, int y);
    bool operator<(const Point &p) const;
//>>>>>>> upstream/master
};

#endif // POINT_H
