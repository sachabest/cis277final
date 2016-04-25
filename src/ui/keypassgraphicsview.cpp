#include "keypassgraphicsview.h"
#include <QKeyEvent>
#include "scene/texture.h"

KeyPassGraphicsView::KeyPassGraphicsView(QWidget *parent) :
    QGraphicsView(parent), gl(nullptr), hudUp(true), hud(nullptr) {
}

void KeyPassGraphicsView::keyPressEvent(QKeyEvent *e) {
    if (gl) {
        gl->keyPressEvent(e);
        if (e->key() == Qt::Key_R) {
            Texture removed = gl->destroyBlocks();
            if (removed != EMPTY) {
                if (hud) {
                    hud->removeBlock(removed);
                }
            }
        } else if (e->key() == Qt::Key_T) {

        }
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
