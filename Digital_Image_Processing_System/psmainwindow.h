#ifndef PSMAINWINDOW_H
#define PSMAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>

#include <bmpimg.h>
#include <headerinfodialog.h>
#include <position.h>
#include <colordisplay.h>
#include <scalingdialog.h>

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
    void receivePosition(QPoint p);

    void receive_scaling(double xScale, double yScale, int method);

    void on_actionOpen_BMP_file_triggered();

    void on_actionDisplay_file_header_triggered();

    void on_actionSave_to_new_BMP_file_triggered();

    void on_actionGet_pixel_value_triggered();

    void on_actionSet_pixel_value_triggered();

    void on_actionImage_interpolation_triggered();

private:
    Ui::PSMainWindow *ui;
    BMPIMG image;
    HeaderInfoDialog headerInfoDialog;
    QPoint p;
};

#endif // PSMAINWINDOW_H
