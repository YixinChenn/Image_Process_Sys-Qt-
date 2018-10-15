#include "headerinfodialog.h"
#include "ui_headerinfodialog.h"

HeaderInfoDialog::HeaderInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeaderInfoDialog)
{
    ui->setupUi(this);
}

HeaderInfoDialog::~HeaderInfoDialog()
{
    delete ui;
}

void HeaderInfoDialog::setInfo(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader)
{
    QString line;
    line = "btType (file type) = " + fileHeader.bfType;
    ui->listWidget->addItem(line);
    line = "bfSize (file length) = " + fileHeader.bfSize;
    ui->listWidget->addItem(line);
    line = "bfOffBits (offset of bit map data in bytes) = " + fileHeader.bfOffBits;
    ui->listWidget->addItem(line);
    line = "biSize (header structure length shoud be 40 or 0x28) = " + infoHeader.biSize;
    ui->listWidget->addItem(line);
}

