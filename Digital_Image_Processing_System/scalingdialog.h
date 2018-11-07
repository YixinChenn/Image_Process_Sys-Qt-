#ifndef SCALINGDIALOG_H
#define SCALINGDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class ScalingDialog;
}

class ScalingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScalingDialog(QWidget *parent = nullptr);
    ~ScalingDialog();

private slots:
    void on_pushButton_clicked();

signals:
    void send_scaling(double xScale, double yScale, int method);//0-最邻近，1-双线性

private:
    Ui::ScalingDialog *ui;
};

#endif // SCALINGDIALOG_H
