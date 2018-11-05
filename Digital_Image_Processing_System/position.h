#ifndef POSITION_H
#define POSITION_H

#include <QDialog>

namespace Ui {
class position;
}

class position : public QDialog
{
    Q_OBJECT

public:
    explicit position(QWidget *parent = nullptr);
    ~position();
    void setMax(int x, int y);

private slots:
    void on_pushButton_clicked();

signals:
    void send_position(QPoint p);

private:
    Ui::position *ui;
};

#endif // POSITION_H
