#include "psmainwindow.h"
#include "ui_psmainwindow.h"

PSMainWindow::PSMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PSMainWindow)
{
    ui->setupUi(this);
}

PSMainWindow::~PSMainWindow()
{
    delete ui;
}

void PSMainWindow::setImg(BMPIMG image)
{
    this->image = image;
    return;
}

void PSMainWindow::on_actionOpen_BMP_file_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.bmp)"));
    if(path.length() == 0) {
        QMessageBox::information(this, tr("Path"), tr("You didn't select any files."));
    }
    BMPIMG image(path);
    setImg(image);
    return;
}


void PSMainWindow::on_actionDisplay_file_header_triggered()
{
    if(image.isEmpty()){

        QMessageBox::information(this, tr("warning"), tr("You didn't open any image, please open an image first."));
        return;
    }
    BITMAPFILEHEADER fileHeader = image.getFileHeader();
    BITMAPINFOHEADER infoHeader = image.getInfoHeader();
    HeaderInfoDialog headerInfoDialog;
    headerInfoDialog.setInfo(fileHeader, infoHeader);
    headerInfoDialog.show();

}
