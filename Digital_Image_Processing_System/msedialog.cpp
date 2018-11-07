#include "msedialog.h"
#include "ui_msedialog.h"

MSEDialog::MSEDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MSEDialog)
{
    ui->setupUi(this);
}

MSEDialog::~MSEDialog()
{
    delete ui;
}

void MSEDialog::setMax(int max)
{
    ui->spinBox->setMaximum(max);
    return;
}

void MSEDialog::on_pushButton_clicked()
{
    emit send_mse(ui->doubleSpinBox->value(), ui->spinBox->value());
    this->close();
    return;
}
