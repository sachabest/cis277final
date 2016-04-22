#include "mainwindow.h"
#include <ui_mainwindow.h>
#include <ui_hud.h>
#include <cameracontrolshelp.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
//    ui->mygl->setFocus();
//    ui->stackedWidget->setst
    MyGL *gl = new MyGL();
    this->ui->graphicsView->setScene(new QGraphicsScene(this->ui->graphicsView));
    this->ui->graphicsView->scene()->addWidget(gl);
    this->ui->graphicsView->gl = gl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::slot_ResizeToCamera(int w, int h)
{
    setFixedWidth(w);
    setFixedWidth(h);
}
void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}
