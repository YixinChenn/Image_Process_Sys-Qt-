#ifndef BMPIMG_H
#define BMPIMG_H

#include <QtCore/QString>
#include <QFile>
#include <QMessageBox>
#include <QDataStream>
#include <QImage>
#include <math.h>

#include <bmpfile.h>

class BMPIMG
{
private:
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD *rgbQuad;
    IMAGEDATA *imgData;

    BYTE getColor(int r, int b, int g);
    IMAGEDATA *getBiliner(double xSrc, double ySrc);
    IMAGEDATA getPixelData(int x, int y);
    void imgDataCpy(IMAGEDATA *src, IMAGEDATA *dst);
    IMAGEDATA *getGaussianColor(int x, int y, int blurRadius, double mse);
    double getGaussian(int x, int y, double mse);

public:
    BMPIMG();//新建BMP文件
    BMPIMG(QString filename);//打开已有BMP文件
    bool isEmpty();
    bool getImage(QString filename);
    void setImage(BMPIMG image);
    BITMAPFILEHEADER getFileHeader();
    BITMAPINFOHEADER getInfoHeader();
    bool saveImage(QString path);
    QColor getPixel(int x, int y);
    void setPixel(int r, int b, int g, int x, int y);

    void nearestInterpolation(double xScale, double yScale);
    void bilinerInterpolation(double xScale, double yScale);
    void medianFiltering();
    void gaussianSmoothing(double mse, int blurRadius);

    QImage toQImage();
};

#endif // BMPIMG_H
