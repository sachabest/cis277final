#include "keypassgraphicsview.h"
#include <QKeyEvent>

KeyPassGraphicsView::KeyPassGraphicsView(QWidget *parent) :
    QGraphicsView(parent), gl(nullptr), hudUp(true), hud(nullptr) {
}

void KeyPassGraphicsView::keyPressEvent(QKeyEvent *e) {
    if (gl) {
        gl->keyPressEvent(e);
        if (scene()) {
            scene()->update();
        }
    }
    if (hud) {
        if (e->key() == Qt::Key_I) {
            hudUp = !hudUp;
            hud->clearFocus();
            hud->setVisible(hudUp);
            scene()->update();
        } else {
            hud->keyPressEvent(e);
            if (scene()) {
                scene()->update();
            }
        }
    }
//    QGraphicsView::keyPressEvent(e);
}
