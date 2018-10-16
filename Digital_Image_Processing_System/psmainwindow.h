#ifndef PSMAINWINDOW_H
#define PSMAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <bmpimg.h>
#include <headerinfodialog.h>

namespace Ui {
class PSMainWindow;
}

class PSMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PSMainWindow(QWidget *parent = nullptr);
    ~PSMainWindow();
    void setImg(BMPIMG image);

private slots:
    void on_actionOpen_BMP_file_triggered();

    void on_actionDisplay_file_header_triggered();

private:
    Ui::PSMainWindow *ui;
    BMPIMG image;
    HeaderInfoDialog headerInfoDialog;
};

#endif // PSMAINWINDOW_H
