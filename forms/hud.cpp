#include "hud.h"
#include "ui_hud.h"

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

    QPalette p = ui->labelTitle->palette();
    p.setColor(QPalette::Background, Qt::transparent);
    ui->labelTitle->setPalette(p);
    for (int i = 0; i < (int) EMPTY; i++) {
        texmap[(Texture) i] = pullTexture((Texture) i);
        selectedTexmap[(Texture) i] = pullTexture((Texture) i);
        QIcon icon(((texmap[(Texture) i])));
        QListWidgetItem *item = new QListWidgetItem(icon, "0", ui->inventory);
        item->setIcon(icon);
        item->setSizeHint(QSize(200, 80));
        ui->inventory->addItem(item);
        itemMap[(Texture) i] = item;
        reverseMap[item] = (Texture) i;
    }
}

void HUD::on_listwidget_select(QListWidgetItem *item) {
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
        for (int i = 0; i < p.width(); i++) {

        }
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
