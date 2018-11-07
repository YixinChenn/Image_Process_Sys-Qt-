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
    ui->listWidget->clear();
    QString line;
    line = "btType (file type) = " + QString::number(fileHeader.bfType);
    ui->listWidget->addItem(line);
    line = "bfSize (file length) = " + QString::number(fileHeader.bfSize);
    ui->listWidget->addItem(line);
    line = "bfOffBits (offset of bit map data in bytes) = " + QString::number(fileHeader.bfOffBits);
    ui->listWidget->addItem(line);
    line = "biSize (header structure length shoud be 40 or 0x28) = " + QString::number(infoHeader.biSize);
    ui->listWidget->addItem(line);
    line = "biWidth (image width) = " + QString::number(infoHeader.biWidth);
    ui->listWidget->addItem(line);
    line = "biHeight (image height) = " + QString::number(infoHeader.biHeight);
    ui->listWidget->addItem(line);
    line = "biPlanes (must be eaual to 1) = " + QString::number(infoHeader.biPlanes);
    ui->listWidget->addItem(line);
    line = "biBitCount (color/pixel bits) = " + QString::number(infoHeader.biBitCount);
    ui->listWidget->addItem(line);
    line = "biCompression (compressed?) = " + QString::number(infoHeader.biCompression);
    ui->listWidget->addItem(line);
    line = "biSizeImage (length of bit map data in bytes must be the times of 4) = " + QString::number(infoHeader.biSizeImage);
    ui->listWidget->addItem(line);
    line = "biXPelsPerMeter (horizontal resolution of target device in pixels/metre) = " + QString::number(infoHeader.biXPelsPerMeter);
    ui->listWidget->addItem(line);
    line = "biYpelsPerMeter (vertical resolution of target device in pixels/metre) = " + QString::number(infoHeader.biYPelsPerMeter);
    ui->listWidget->addItem(line);
    line = "biColorUsed (number of colors used in bitmap, 0 = 2**biBitCount) = " + QString::number(infoHeader.biClrUsed);
    ui->listWidget->addItem(line);
    line = "biColorImportant (number of important colors, 0 = all colors are impretant) = " + QString::number(infoHeader.biClrImportant);
    ui->listWidget->addItem(line);

    line = "";
    ui->listWidget->addItem(line);

    line = "The following is additional information:";
    ui->listWidget->addItem(line);
    line = "Bytes per row in bitmap (nBytesPerRow) = " + QString::number(infoHeader.biWidth * (infoHeader.biBitCount/8));
    ui->listWidget->addItem(line);
    line = "Total bytes of bitmap (nImageSizeInByte) = " + QString::number(fileHeader.bfSize - fileHeader.bfOffBits);
    ui->listWidget->addItem(line);
    line = "Actual pixels per row in bitmap (nPixelsPerRpe)= " + QString::number(infoHeader.biWidth);
    ui->listWidget->addItem(line);
    line = "Total rows of bitmap (nTotalRows) = " + QString::number(infoHeader.biHeight);
    ui->listWidget->addItem(line);
    line = "Total colors (2**biBitCount)(nTotalColors) = " + QString::number(pow(2, infoHeader.biBitCount));
    ui->listWidget->addItem(line);
    line = "Used colors (biColorUsed)(nUsedolors) = " + QString::number(infoHeader.biClrUsed);
    ui->listWidget->addItem(line);
}


