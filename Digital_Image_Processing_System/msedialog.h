#ifndef MSEDIALOG_H
#define MSEDIALOG_H

#include <QDialog>

namespace Ui {
class MSEDialog;
}

class MSEDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MSEDialog(QWidget *parent = nullptr);
    ~MSEDialog();
    void setMax(int max);

signals:
    void send_mse(double mse, int blurRadius);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MSEDialog *ui;
};

#endif // MSEDIALOG_H
