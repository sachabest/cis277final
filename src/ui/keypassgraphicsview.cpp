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
                    SoundManager::playOff();
                }
            } else {
                SoundManager::playMiss();
            }
        } else if (e->key() == Qt::Key_T) {
            if (hud && gl->canAddBlock()) {
                Texture toAdd = hud->addBlock();
                if (toAdd != EMPTY) {
                    gl->sachaAddBlock(toAdd);
                    SoundManager::playOn();
                } else {
                    SoundManager::playMiss();
                }
            } else {
                SoundManager::playMiss();
            }
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
