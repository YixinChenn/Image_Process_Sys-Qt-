#include "psmainwindow.h"
#include "ui_psmainwindow.h"

PSMainWindow::PSMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PSMainWindow)
{
    ui->setupUi(this);
}

PSMainWindow::~PSMainWindow()
{
    delete ui;
}
