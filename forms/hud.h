#ifndef HUD_H
#define HUD_H

#include <QWidget>

namespace Ui {
class HUD;
}

class HUD : public QWidget
{
    Q_OBJECT

public:
    explicit HUD(QWidget *parent = 0);
    ~HUD();

private:
    Ui::HUD *ui;
};

#endif // HUD_H
