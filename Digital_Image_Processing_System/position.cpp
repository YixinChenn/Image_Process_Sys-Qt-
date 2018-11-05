#include "position.h"
#include "ui_position.h"

position::position(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::position)
{
    ui->setupUi(this);
}

position::~position()
{
    delete ui;
}

void position::setMax(int x, int y)
{
    ui->xInput->setMaximum(x);
    ui->yInput->setMaximum(y);
}


void position::on_pushButton_clicked()
{
    int x = ui->xInput->value();
    int y = ui->yInput->value();
    QPoint p(x,y);
    emit send_position(p);
    this->close();
}
