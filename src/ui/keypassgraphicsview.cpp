#include "keypassgraphicsview.h"

KeyPassGraphicsView::KeyPassGraphicsView(QWidget *parent) : QGraphicsView(parent), gl(nullptr) {
}

void KeyPassGraphicsView::keyPressEvent(QKeyEvent *e) {
    if (gl) {
        gl->keyPressEvent(e);
        if (scene()) {
            scene()->update();
        }
    }
    QGraphicsView::keyPressEvent(e);
}
