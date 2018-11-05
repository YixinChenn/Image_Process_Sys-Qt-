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

void PSMainWindow::receivePosition(QPoint p)
{
    this->p = p;
}

void PSMainWindow::on_actionOpen_BMP_file_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.bmp)"));
    if(path.length() == 0) {
        QMessageBox::information(this, tr("Path"), tr("You didn't select any files."));
    }
    BMPIMG image(path);
    setImg(image);
    QImage qImage = image.toQImage();
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addPixmap(QPixmap::fromImage(qImage));
    qDebug()<<qImage;
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
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
    headerInfoDialog.setInfo(fileHeader, infoHeader);
    headerInfoDialog.show();

}

void PSMainWindow::on_actionSave_to_new_BMP_file_triggered()
{
    if(image.isEmpty()){
        QMessageBox::information(this, tr("warning"), tr("Please open an image first."));
        return;
    }
    QString path = QFileDialog::getSaveFileName(this, tr("Save Image"), " ", tr("Image Files(*.bmp)"));
    if(!path.isNull()){
        image.saveImage(path);
    }
    else{
        QMessageBox::information(this, tr("Path"), tr("You didn't input a file name."));
    }
}

void PSMainWindow::on_actionGet_pixel_value_triggered()
{
    position *dlg = new position(this);
    connect(dlg, SIGNAL(send_position(QPoint)), this, SLOT(receivePosition(QPoint)));
    dlg->setMax(image.getInfoHeader().biWidth, image.getInfoHeader().biHeight);
    dlg->exec();
    int r, g, b;
    QColor pix = image.getPixel(p.x(), p.y());
    colorDisplay *displayDialog = new colorDisplay();
    displayDialog->setInfo(pix);
    displayDialog->show();
}
