#ifndef HEADERINFODIALOG_H
#define HEADERINFODIALOG_H

#include <QDialog>

#include <bmpfile.h>

namespace Ui {
class HeaderInfoDialog;
}

class HeaderInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeaderInfoDialog(QWidget *parent = nullptr);
    ~HeaderInfoDialog();
    void setInfo(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader);

private:
    Ui::HeaderInfoDialog *ui;
};

#endif // HEADERINFODIALOG_H
