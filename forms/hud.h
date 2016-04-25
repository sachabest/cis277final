#ifndef HUD_H
#define HUD_H

#include <QWidget>
#include <QListWidgetItem>
#include <QBitmap>
#include "scene/texture.h"
#include "la.h"

namespace Ui {
class HUD;
}

class HUD : public QWidget
{
    Q_OBJECT

public:
    explicit HUD(QWidget *parent = 0);
    ~HUD();
    void keyPressEvent(QKeyEvent *);
    Texture addBlock();
    void removeBlock(Texture t);
protected:
    void on_listwidget_select(QListWidgetItem *item);
private:
    Ui::HUD *ui;
    QVector<QListWidgetItem *> blocks;
    Texture currentSelected;
    QImage createSubImage(QImage &img, const QRect &rect);
    QPixmap pullTexture(Texture t, bool edged = false);
    QMap<Texture, QPixmap> texmap;
    QMap<Texture, QPixmap> selectedTexmap;
    QMap<Texture, QListWidgetItem *> itemMap;
    QMap<QListWidgetItem *, Texture> reverseMap;
    void addBlockToInventory(Texture t);
    void removeBlockFromInventory(Texture t);
    int getBlockCount(Texture t);
};

#endif // HUD_H
