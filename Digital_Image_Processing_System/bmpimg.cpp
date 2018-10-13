#include "bmpimg.h"

BMPIMG::BMPIMG(QString filename)
{
    char *buf;
    char *p;
    int r,g,b,pix;
    //open file
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){//open file failed
        QMessageBox::warning(0, "Waring", "open file " + filename + " failed!", QMessageBox::Yes);
        return;
    }

    //read file header
    QDataStream dataStream(&file);
    dataStream>>fileHeader.bfType;
    if(fileHeader.bfType != 0x424D){//bfType != "BM"
        QMessageBox::warning(0, "Waring", "file " + filename + " is not a BMP image!", QMessageBox::Yes);
        return;
    }
    dataStream>>fileHeader.bfSize;
    dataStream>>fileHeader.bfReserved1;
    dataStream>>fileHeader.bfReserved2;
    dataStream>>fileHeader.bfOffBits;

    //read info header
    dataStream>>infoHeader.biSize;
    dataStream>>infoHeader.biWidth;
    dataStream>>infoHeader.biHeight;
    dataStream>>infoHeader.biPlanes;
    if(infoHeader.biPlanes != 1){
        QMessageBox::warning(0, "Waring", "biPlanes is not 1!", QMessageBox::Yes);
        return;
    }
    dataStream>>infoHeader.biBitCount;
    dataStream>>infoHeader.biCompression; //压缩类型
    dataStream>>infoHeader.biSizeImage; //压缩图像大小字节数
    dataStream>>infoHeader.biXPelsPerMeter; //水平分辨率
    dataStream>>infoHeader.biYPelsPerMeter; //垂直分辨率
    dataStream>>infoHeader.biClrUsed; //位图实际用到的色彩数
    dataStream>>infoHeader.biClrImportant; //本位图中重要的色彩数

    //read rgbquad
    RGBQUAD tempQuad[infoHeader.biClrUsed];
    for(unsigned int nCounti=0; nCounti<infoHeader.biClrUsed; nCounti++){
        dataStream>>tempQuad[nCounti].rgbBlue;
        dataStream>>tempQuad[nCounti].rgbGreen;
        dataStream>>tempQuad[nCounti].rgbRed;
        dataStream>>tempQuad[nCounti].rgbReserved;
    }
    rgbQuad = tempQuad;

    //read image data



}
