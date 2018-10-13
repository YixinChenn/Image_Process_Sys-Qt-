#ifndef PSMAINWINDOW_H
#define PSMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class PSMainWindow;
}

class PSMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PSMainWindow(QWidget *parent = nullptr);
    ~PSMainWindow();

private:
    Ui::PSMainWindow *ui;
};

#endif // PSMAINWINDOW_H
