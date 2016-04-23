#include "hud.h"
#include "ui_hud.h"
#include <QKeyEvent>

HUD::HUD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HUD)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_PaintOnScreen);

    setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->inventory->setIconSize(QSize(60, 60));
    ui->inventory->setAutoFillBackground(false);
    QPalette palette = ui->inventory->palette();
    palette.setColor(QPalette::Base, QColor(255, 255, 255, 50));
    palette.setColor(QPalette::Highlight, QColor(255, 0, 255, 50));
    ui->inventory->setPalette(palette);
    ui->inventory->setAttribute(Qt::WA_NoSystemBackground);
    ui->inventory->setAttribute(Qt::WA_TranslucentBackground);
    ui->inventory->setAttribute(Qt::WA_TransparentForMouseEvents);
    QObject::connect(ui->inventory, &QListWidget::currentItemChanged, this, &HUD::on_listwidget_select);
    QPalette p = ui->labelTitle->palette();
    p.setColor(QPalette::Background, Qt::transparent);
    ui->labelTitle->setPalette(p);
    for (int i = 0; i < (int) EMPTY; i++) {
        texmap[(Texture) i] = pullTexture((Texture) i);
        selectedTexmap[(Texture) i] = pullTexture((Texture) i, true);
        QIcon icon(((texmap[(Texture) i])));
        QListWidgetItem *item = new QListWidgetItem(icon, "0", ui->inventory);
        item->setIcon(icon);
        item->setSizeHint(QSize(200, 80));
        ui->inventory->addItem(item);
        itemMap[(Texture) i] = item;
        reverseMap[item] = (Texture) i;
    }
    currentSelected = (Texture) 0;
    ui->inventory->item(0)->setIcon(QIcon(selectedTexmap[currentSelected]));
}

void HUD::keyPressEvent(QKeyEvent *e) {
    if( e->modifiers() & Qt::ShiftModifier ) {
        qDebug() << e->key();
        Texture blockType = EMPTY;
        // I originally did this with modulo math, but the Macbook keyboard doesn't have the number
        // keys ordered sequentially. So we have do a manual switch. Fuck you Qt
        if (e->text() == "!") {
            blockType = (Texture) 0;
        } else if (e->text() == "@") {
            blockType = (Texture) 1;
        } else if (e->text() == "#") {
            blockType = (Texture) 2;
        } else if (e->text() == "$") {
            blockType = (Texture) 3;
        } else if (e->text() == "%") {
            blockType = (Texture) 4;
        } else if (e->text() == "^") {
            blockType = (Texture) 5;
        } else if (e->text() == "&") {
            blockType = (Texture) 6;
        }
        if ((int) blockType < 0 || (int) blockType >= (int) EMPTY || blockType == currentSelected) {
            return;
        }
        if ((int) currentSelected < ui->inventory->count() && (int) currentSelected >= 0) {
            ui->inventory->item((int) currentSelected)->setIcon(QIcon(texmap[currentSelected]));
        }
        if ((int) blockType < ui->inventory->count() && (int) blockType >= 0) {
            ui->inventory->item((int) blockType)->setIcon(QIcon(selectedTexmap[blockType]));
        }
        currentSelected = blockType;
    }
}

void HUD::on_listwidget_select(QListWidgetItem *item) {
    Texture t = reverseMap[item];
    item->setIcon(QIcon(((selectedTexmap[(Texture) t]))));
}

QImage HUD::createSubImage(QImage &image, const QRect &rect) {
    size_t offset = rect.x() * image.depth() / 8
                    + rect.y() * image.bytesPerLine();
    return QImage(image.bits() + offset, rect.width(), rect.height(),
                  image.bytesPerLine(), image.format());
}

QPixmap HUD::pullTexture(Texture t, bool edged) {
    QRect rect;
    QPixmap base(":/minecraft_textures_all.png");
    int size = base.width() / 16;
    switch (t) {
    case STONE:
        rect = QRect(size, 0, size, size);
        break;
    case WOOD:
        rect = QRect(4 * size, size, size, size);
        break;
    case GRASS:
        rect = QRect(3 * size, 0, size, size);
    case EMPTY:
    default:
        break;
    }
    QPixmap p = base.copy(rect).scaled(128, 128);
    if (edged) {
        QImage img = p.toImage();
        int h = img.height() - 1;
        int w = img.width() - 1;
        int c = qRgba(255, 255, 0, 255);
        int edgeSize = 10;
        for (int i = 0; i < p.width(); i++) {
            for (int j = 0; j < edgeSize; j++) {
                img.setPixel(i, j, c);
                img.setPixel(i, h - j, c);
            }
        }
        for (int j = 0; j < p.height(); j++) {
            for (int i = 0; i < edgeSize; i++) {
                img.setPixel(i, j, c);
                img.setPixel(w -i, j, c);
            }
        }
        p = QPixmap::fromImage(img);
    }
    return p;
}

void HUD::removeBlockFromInventory(Texture t) {
    itemMap[t]->setText(QString::number(itemMap[t]->text().toInt() - 1));
}

void HUD::addBlockToInventory(Texture t) {
    itemMap[t]->setText(QString::number(itemMap[t]->text().toInt() + 1));
}

HUD::~HUD()
{
    delete ui;
    for (QListWidgetItem *i : itemMap.values()) {
        delete i;
    }
}
