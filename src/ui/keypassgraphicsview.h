#ifndef KEYPASSGRAPHICSVIEW_H
#define KEYPASSGRAPHICSVIEW_H

#include <QGraphicsView>
#include "mygl.h"
#include "forms/hud.h"
#include "soundmanager.h"

class KeyPassGraphicsView : public QGraphicsView
{
public:
    KeyPassGraphicsView(QWidget *parent = nullptr);
    MyGL *gl;
    HUD *hud;
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    bool hudUp;
};

#endif // KEYPASSGRAPHICSVIEW_H
