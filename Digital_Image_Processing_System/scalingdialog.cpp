#include "scalingdialog.h"
#include "ui_scalingdialog.h"

ScalingDialog::ScalingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScalingDialog)
{
    ui->setupUi(this);
}

ScalingDialog::~ScalingDialog()
{
    delete ui;
}

void ScalingDialog::on_pushButton_clicked()
{
    double xScale = ui->xScale->value();
    double yScale = ui->yScale->value();
    QString methodStr = ui->comboBox->currentText();
    int method = 0;
    if(methodStr == "最邻近差值"){
        method = 0;
    }
    else{
        method = 1;
    }
    qDebug()<<"send: xScale = "<<QString::number(xScale)<<", yScale = "<<QString::number(yScale)<<", method = "<<methodStr;
    emit send_scaling(xScale, yScale, method);
    this->close();
}
