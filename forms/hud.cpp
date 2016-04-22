#include "hud.h"
#include "ui_hud.h"

HUD::HUD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HUD)
{
    ui->setupUi(this);
}

HUD::~HUD()
{
    delete ui;
}
