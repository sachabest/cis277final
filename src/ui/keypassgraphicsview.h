#ifndef KEYPASSGRAPHICSVIEW_H
#define KEYPASSGRAPHICSVIEW_H

#include <QGraphicsView>
#include "mygl.h"

class KeyPassGraphicsView : public QGraphicsView
{
public:
    KeyPassGraphicsView(QWidget *parent = nullptr);
    MyGL *gl;
protected:
    void keyPressEvent(QKeyEvent *e);
};

#endif // KEYPASSGRAPHICSVIEW_H
