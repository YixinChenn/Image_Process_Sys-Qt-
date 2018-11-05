#include "colordisplay.h"
#include "ui_colordisplay.h"

colorDisplay::colorDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::colorDisplay)
{
    ui->setupUi(this);
}

void colorDisplay::setInfo(QColor color){
    ui->Red->setText(QString::number(color.red()));
    ui->Green->setText(QString::number(color.green()));
    ui->Blue->setText(QString::number(color.blue()));
    QImage *colorBlock = new QImage(40, 40, QImage::Format_A2BGR30_Premultiplied);
    colorBlock->fill(color);
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addPixmap(QPixmap::fromImage(*colorBlock));
    ui->graphicsView->setScene(scene);
}

colorDisplay::~colorDisplay()
{
    delete ui;
}
