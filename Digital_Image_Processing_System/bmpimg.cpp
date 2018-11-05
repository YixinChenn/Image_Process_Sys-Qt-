#include "bmpimg.h"

BMPIMG::BMPIMG()
{
    fileHeader.bfType = 0;
}

BMPIMG::BMPIMG(QString filename)
{
    if(!getImage(filename)){
        fileHeader.bfType = 0;
    }
}

bool BMPIMG::isEmpty()
{
    if(fileHeader.bfType == 0x424D){
        return false;
    }
    else{
        return true;
    }
}

bool BMPIMG::getImage(QString filename)
{
    qDebug()<<"open file: " + filename;
    //open file
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){//open file failed
        QMessageBox::warning(0, "Waring", "open file " + filename + " failed!", QMessageBox::Yes);
        return false;
    }

    //read file header
    qDebug()<<"reading file header...";
    QDataStream dataStream(&file);
    dataStream>>fileHeader.bfType;

    if(fileHeader.bfType != 0x424D){//bfType != "BM"
        QMessageBox::warning(0, "Waring", "file " + filename + " is not a BMP image!", QMessageBox::Yes);
        return false;
    }
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream>>fileHeader.bfSize;
    dataStream>>fileHeader.bfReserved1;
    dataStream>>fileHeader.bfReserved2;
    dataStream>>fileHeader.bfOffBits;

    //read info header
    qDebug()<<"reading info header...";
    dataStream>>infoHeader.biSize;
    dataStream>>infoHeader.biWidth;
    dataStream>>infoHeader.biHeight;
    dataStream>>infoHeader.biPlanes;
    dataStream>>infoHeader.biBitCount;
    qDebug()<<"bibitccount = " + QString::number(infoHeader.biBitCount);
    dataStream>>infoHeader.biCompression; //压缩类型
    dataStream>>infoHeader.biSizeImage; //压缩图像大小字节数
    dataStream>>infoHeader.biXPelsPerMeter; //水平分辨率
    dataStream>>infoHeader.biYPelsPerMeter; //垂直分辨率
    dataStream>>infoHeader.biClrUsed; //位图实际用到的色彩数
    if(infoHeader.biClrUsed == 0 && infoHeader.biBitCount == 8)
        infoHeader.biClrUsed = 256;
    dataStream>>infoHeader.biClrImportant; //本位图中重要的色彩数

    if(infoHeader.biBitCount != 24){
        //read rgbquad
        qDebug()<<"reading rgbquad...";
        rgbQuad = (RGBQUAD*)malloc(sizeof(RGBQUAD) * infoHeader.biClrUsed);
        for(unsigned int nCounti=0; nCounti<infoHeader.biClrUsed; nCounti++){
            dataStream>>(*(rgbQuad + nCounti)).rgbBlue;
            dataStream>>(*(rgbQuad + nCounti)).rgbGreen;
            dataStream>>(*(rgbQuad + nCounti)).rgbRed;
            dataStream>>(*(rgbQuad + nCounti)).rgbReserved;
        }
    }

    //read image data
    qDebug()<<"reading image data...";
    imgData = (IMAGEDATA*)malloc(sizeof(IMAGEDATA) * infoHeader.biWidth * infoHeader.biHeight);
    switch (infoHeader.biBitCount) {
        case 8:
            for(int i = 0; i < infoHeader.biHeight; i++){
                for(int j = 0; j < infoHeader.biWidth; j++){
                    dataStream>>(*(imgData + i * infoHeader.biWidth + j)).blue;
                }
            }
        break;
        case 24:
            for(int i = 0; i < infoHeader.biHeight; i++){
                for(int j = 0; j < infoHeader.biWidth; j++){
                    dataStream>>(*(imgData + i * infoHeader.biWidth + j)).blue;
                    dataStream>>(*(imgData + i * infoHeader.biWidth + j)).green;
                    dataStream>>(*(imgData + i * infoHeader.biWidth + j)).red;
                }
            }
        break;
    }
    file.close();
    return true;
}

BITMAPFILEHEADER BMPIMG::getFileHeader()
{
    return fileHeader;
}

BITMAPINFOHEADER BMPIMG::getInfoHeader()
{
    return infoHeader;
}

QImage BMPIMG::toQImage()
{
    QImage outputImg = QImage(infoHeader.biWidth, infoHeader.biHeight, QImage::Format_ARGB32);
    if(infoHeader.biBitCount == 24){
        for(int i=infoHeader.biHeight-1; i>=0; i--){
            for(int j=0; j<infoHeader.biWidth; j++){
                QPoint pos = QPoint(j,i);
                QColor color = QColor(imgData->red, imgData->green, imgData->blue);
                outputImg.setPixelColor(pos, color);
                imgData++;
            }
        }
    }
    else{
        for(int i=infoHeader.biHeight-1; i>=0; i--){
            for(int j=0; j<infoHeader.biWidth; j++){
                QPoint pos = QPoint(j,i);
                BYTE rgb = imgData->blue;
                QColor color = QColor((rgbQuad + rgb)->rgbRed, (rgbQuad + rgb)->rgbGreen, (rgbQuad + rgb)->rgbBlue);
                outputImg.setPixelColor(pos, color);
                imgData++;
            }
        }
    }

    return outputImg;
}

bool BMPIMG::saveImage(QString path){
    QFile newImg(path);
    if(!newImg.open(QIODevice::WriteOnly)){
        QMessageBox::warning(0, "Waring", "save file " + path + " failed!", QMessageBox::Yes);
        return false;
    }
    QDataStream dataStream(&newImg);
    dataStream<<fileHeader.bfType;
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream<<fileHeader.bfSize;
    dataStream<<fileHeader.bfReserved1;
    dataStream<<fileHeader.bfReserved2;
    dataStream<<fileHeader.bfOffBits;

    //write info header
    dataStream<<infoHeader.biSize;
    dataStream<<infoHeader.biWidth;
    dataStream<<infoHeader.biHeight;
    dataStream<<infoHeader.biPlanes;
    dataStream<<infoHeader.biBitCount;
    dataStream<<infoHeader.biCompression; //压缩类型
    dataStream<<infoHeader.biSizeImage; //压缩图像大小字节数
    dataStream<<infoHeader.biXPelsPerMeter; //水平分辨率
    dataStream<<infoHeader.biYPelsPerMeter; //垂直分辨率
    dataStream<<infoHeader.biClrUsed; //位图实际用到的色彩数
    dataStream<<infoHeader.biClrImportant; //本位图中重要的色彩数

    if(infoHeader.biBitCount != 24){
        //write rgbquad
        for(unsigned int nCounti=0; nCounti<infoHeader.biClrUsed; nCounti++){
            dataStream<<(*(rgbQuad + nCounti)).rgbBlue;
            dataStream<<(*(rgbQuad + nCounti)).rgbGreen;
            dataStream<<(*(rgbQuad + nCounti)).rgbRed;
            dataStream<<(*(rgbQuad + nCounti)).rgbReserved;
        }
    }

    //write image data
    switch (infoHeader.biBitCount) {
        case 8:
            for(int i = 0; i < infoHeader.biWidth * infoHeader.biHeight; i++){
                if(!imgData){
                    qDebug()<<"A";
                }
                dataStream<<imgData->blue;
                imgData++;
            }
        break;
        case 24:
            for(int i = 0; i < infoHeader.biWidth * infoHeader.biHeight; i++){
                dataStream<<imgData->blue;
                dataStream<<imgData->green;
                dataStream<<imgData->red;
                imgData++;
            }
        break;
    }
    newImg.close();
    return true;
}

QColor BMPIMG::getPixel(int x, int y)
{
    IMAGEDATA pix = *(imgData + y * infoHeader.biWidth + x);
    QColor color;
    switch(infoHeader.biBitCount){
        case 8:
            color.setRed((rgbQuad + pix.blue)->rgbRed);
            color.setBlue((rgbQuad + pix.blue)->rgbBlue);
            color.setGreen((rgbQuad + pix.blue)->rgbGreen);
        break;
        case 24:
            color.setRed(pix.red);
            color.setBlue(pix.blue);
            color.setGreen(pix.green);
        break;
    }
    return color;
}
