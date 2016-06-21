#include "zmainwindow.h"
#include "ui_zmainwindow.h"

zMainWindow::zMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::zMainWindow)
{
    ui->setupUi(this);
}

zMainWindow::~zMainWindow()
{
    delete ui;
}
