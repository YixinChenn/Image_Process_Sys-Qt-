#include "bmpimg.h"

/*
 * find the most similar color in RBGQUAD with the given color
*/
BYTE BMPIMG::getColor(int r, int b, int g)
{
    BYTE color = 0;
    int similarity = 255 * 3;
    int temp;
    for(int i=0; i<256; i++){
        temp = abs((rgbQuad+i)->rgbRed - r) + abs((rgbQuad+i)->rgbBlue - b) + abs((rgbQuad+i)->rgbGreen - g);
        if(temp < similarity){
            similarity = temp;
            color = (BYTE)i;
            if(similarity == 0) break;
        }
    }
    return color;
}

/*
 * calculate biliner color
*/
IMAGEDATA *BMPIMG::getBiliner(double xSrc, double ySrc)
{
    IMAGEDATA *color = (IMAGEDATA*)malloc(sizeof(IMAGEDATA));
    int xF = (int)floor(xSrc);
    int yF = (int)floor(ySrc);
    int xC = (int)ceil(xSrc);
    int yC = (int)ceil(ySrc);
    IMAGEDATA xFyF = *(imgData + yF * infoHeader.biWidth + xF);
    IMAGEDATA xCyF = *(imgData + yF * infoHeader.biWidth + xC);
    IMAGEDATA xFyC = *(imgData + yC * infoHeader.biWidth + xF);
    IMAGEDATA xCyC = *(imgData + yC * infoHeader.biWidth + xC);
    color->blue = (BYTE)round(xFyF.blue*(xC-xSrc)*(yC-ySrc) + xCyF.blue*(xSrc-xF)*(yC-ySrc) + xFyC.blue*(xC-xSrc)*(ySrc-yF) + xCyC.blue*(xSrc-xF)*(ySrc-yF));
    color->green = (BYTE)round(xFyF.green*(xC-xSrc)*(yC-ySrc) + xCyF.green*(xSrc-xF)*(yC-ySrc) + xFyC.green*(xC-xSrc)*(ySrc-yF) + xCyC.green*(xSrc-xF)*(ySrc-yF));
    color->red = (BYTE)round(xFyF.red*(xC-xSrc)*(yC-ySrc) + xCyF.red*(xSrc-xF)*(yC-ySrc) + xFyC.red*(xC-xSrc)*(ySrc-yF) + xCyC.red*(xSrc-xF)*(ySrc-yF));
    return color;
}

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

/*
 * @param filename   // the path of the BMP file
 */

bool BMPIMG::getImage(QString filename)
{
    //open file
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){//open file failed
        QMessageBox::warning(0, "Waring", "open file " + filename + " failed!", QMessageBox::Yes);
        return false;
    }

    //read file header
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
    dataStream>>infoHeader.biSize;
    dataStream>>infoHeader.biWidth;
    dataStream>>infoHeader.biHeight;
    dataStream>>infoHeader.biPlanes;
    dataStream>>infoHeader.biBitCount;
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
        rgbQuad = (RGBQUAD*)malloc(sizeof(RGBQUAD) * infoHeader.biClrUsed);
        for(unsigned int nCounti=0; nCounti<infoHeader.biClrUsed; nCounti++){
            dataStream>>(*(rgbQuad + nCounti)).rgbBlue;
            dataStream>>(*(rgbQuad + nCounti)).rgbGreen;
            dataStream>>(*(rgbQuad + nCounti)).rgbRed;
            dataStream>>(*(rgbQuad + nCounti)).rgbReserved;
        }
    }

    //read image data
    imgData = (IMAGEDATA*)malloc(sizeof(IMAGEDATA) * infoHeader.biWidth * infoHeader.biHeight);
    int cnt = 0;
    int align = (4 - (int)infoHeader.biWidth % 4) % 4;
    IMAGEDATA temp;
    switch (infoHeader.biBitCount) {
        case 8:
            for(int i = 0; i < infoHeader.biHeight; i++){
                for(int j = 0; j < infoHeader.biWidth; j++){
                    dataStream>>(*(imgData + cnt)).blue;
                    cnt++;
                }
                if(align!=0){
                    for(int k=0; k<align; k++){
                        dataStream >> temp.blue;
                    }
                }

            }
        break;
        case 24:
            for(int i = 0; i < infoHeader.biHeight; i++){
                for(int j = 0; j < infoHeader.biWidth; j++){
                    dataStream>>(*(imgData + cnt)).blue;
                    dataStream>>(*(imgData + cnt)).green;
                    dataStream>>(*(imgData + cnt)).red;
                    cnt++;
                }
                if(align!=0){
                    for(int k=0; k<align; k++){
                        dataStream >> temp.blue;
                        dataStream >> temp.green;
                        dataStream >> temp.red;
                    }
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

/*
 * @return displayable QImage
*/

QImage BMPIMG::toQImage()
{
    int cnt = 0;
    QImage outputImg = QImage(infoHeader.biWidth, infoHeader.biHeight, QImage::Format_ARGB32);
    QPoint pos;
    QColor color;
    BYTE  rgb;
    if(infoHeader.biBitCount == 24){
        for(int i=infoHeader.biHeight-1; i>=0; i--){
            for(int j=0; j<infoHeader.biWidth; j++){
                pos = QPoint(j,i);
                color = QColor((imgData + cnt)->red, (imgData + cnt)->green, (imgData + cnt)->blue);
                outputImg.setPixelColor(pos, color);
                cnt++;
            }
        }
    }
    else{
        for(int i=infoHeader.biHeight-1; i>=0; i--){
            for(int j=0; j<infoHeader.biWidth; j++){
                pos = QPoint(j,i);
                rgb = (imgData + cnt)->blue;
                color = QColor((rgbQuad + rgb)->rgbRed, (rgbQuad + rgb)->rgbGreen, (rgbQuad + rgb)->rgbBlue);
                outputImg.setPixelColor(pos, color);
                cnt++;
            }
        }
    }

    return outputImg;
}

/*
 * @param path  //the path of the new image
*/

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
    int cnt = 0;
    int alignByte = (4 - (int)infoHeader.biWidth % 4) % 4;
    IMAGEDATA align;
    align.blue = 0;
    align.green = 0;
    align.red = 0;
    switch (infoHeader.biBitCount) {
        case 8:
            for(int i = 0; i < infoHeader.biHeight; i++){
                for(int j = 0; j < infoHeader.biWidth; j++){
                    dataStream<<(imgData+cnt)->blue;
                    cnt++;
                }
                if(alignByte != 0){
                    for(int k = 0; k < alignByte; k++){
                        dataStream<<align.blue;
                    }
                }
            }
        break;
        case 24:
            for(int i = 0; i < infoHeader.biHeight; i++){
                for(int j = 0; j < infoHeader.biWidth; j++){
                    dataStream<<(imgData+cnt)->blue;
                    dataStream<<(imgData+cnt)->green;
                    dataStream<<(imgData+cnt)->red;
                    cnt++;
                }
                if(alignByte != 0){
                    for(int k = 0; k < alignByte; k++){
                        dataStream<<align.blue;
                    }
                }
            }
        break;
    }
    newImg.close();
    return true;
}

QColor BMPIMG::getPixel(int x, int y)
{
    IMAGEDATA pix = getPixelData(x, y);
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

/*
 * @param x //the x-coordinate
 * @param y //the y-coordinate
 * @return the IMAGEDATA needed
*/
IMAGEDATA BMPIMG::getPixelData(int x, int y)
{
    IMAGEDATA pix = *(imgData + y * infoHeader.biWidth + x);
    return pix;
}

/*
 * copy src to dst
*/
void BMPIMG::imgDataCpy(IMAGEDATA *src, IMAGEDATA *dst)
{
    dst->blue = src->blue;
    dst->green = src->green;
    dst->red = src-> red;
    return;
}

/*
 * calculate every pixel's color in gaussian smoothing
*/
IMAGEDATA *BMPIMG::getGaussianColor(int x, int y, int blurRadius, double mse)
{
    IMAGEDATA *color = (IMAGEDATA*)malloc(sizeof(IMAGEDATA));
    color->blue = 0;
    color->red = 0;
    color->green = 0;
    IMAGEDATA pix;
    double gaussian= 0, sum = 0;
    double gaussianMatrix[blurRadius*2+1][blurRadius*2+1];
    for(int i = y-blurRadius; i <= y+blurRadius; i++){
        for(int j = x-blurRadius; j <= x+blurRadius; j++){
            gaussian = getGaussian(j-x,i-y,mse);
            gaussianMatrix[j-x+blurRadius][i-y+blurRadius] = gaussian;
            sum += gaussian;
        }
    }
    for(int i = y-blurRadius; i <= y+blurRadius; i++){
        for(int j = x-blurRadius; j <= x+blurRadius; j++){
            gaussian = gaussianMatrix[j-x+blurRadius][i-y+blurRadius]/sum;
            pix = getPixelData(j, i);
            if(infoHeader.biBitCount == 24){
                color->green += gaussian * pix.green;
                color->red += gaussian * pix.red;
            }
            color->blue += gaussian * pix.blue;
        }
    }

    return color;
}

/*
 * calculate the gaussian of given x, y, and MSE
*/
double BMPIMG::getGaussian(int x, int y, double mse)
{
    double rst = pow(M_E, (-1)*(pow(x,2)+pow(y,2))/(2*pow(mse,2)))/(2*M_PI*pow(mse,2));
    return rst;
}

void BMPIMG::setPixel(int r, int b, int g, int x, int y)
{
    if(infoHeader.biBitCount == 8){
        BYTE color = getColor(r,b,g);
        (imgData + y * infoHeader.biWidth + x)->blue = color;
    }
    else{
        (imgData + y * infoHeader.biWidth + x)->blue = b;
        (imgData + y * infoHeader.biWidth + x)->green = g;
        (imgData + y * infoHeader.biWidth + x)->red = r;
    }
    return;
}

void BMPIMG::nearestInterpolation(double xScale, double yScale)
{
    qDebug()<<"nearest interpolation";
    IMAGEDATA *dstImgData;
    LONG dstWidth = (LONG)round(infoHeader.biWidth * xScale);
    LONG dstHeight = (LONG)round(infoHeader.biHeight * yScale);

    IMAGEDATA *src;
    int xSrc, ySrc;
    dstImgData = (IMAGEDATA*)malloc(sizeof(IMAGEDATA) * dstWidth * dstHeight);
    for(int i=0; i<dstHeight; i++){
        for(int j=0; j<dstWidth; j++){
            xSrc = (int)round((double)j/xScale);
            ySrc = (int)round((double)i/yScale);
            src = (imgData + ySrc * infoHeader.biWidth + xSrc);//我是傻逼。confirmed.
            imgDataCpy(src, (dstImgData + i*dstWidth + j));
        }
    }
    imgData = dstImgData;
    infoHeader.biHeight = dstHeight;
    infoHeader.biWidth = dstWidth;
    infoHeader.biSizeImage = dstHeight * (dstWidth + 4 - dstWidth % 4);
    fileHeader.bfSize = infoHeader.biSizeImage + fileHeader.bfOffBits;

}

void BMPIMG::bilinerInterpolation(double xScale, double yScale)
{
    LONG dstWidth = (LONG)round(infoHeader.biWidth * xScale);
    LONG dstHeight = (LONG)round(infoHeader.biHeight * yScale);

    IMAGEDATA *src;
    double xSrc, ySrc;
    IMAGEDATA *dstImgData = (IMAGEDATA*)malloc(sizeof(IMAGEDATA) * dstWidth * dstHeight);
    for(int i = 0; i < dstHeight; i++){
        for(int j = 0; j < dstWidth; j++){
            xSrc = ((double)j + 0.5)/xScale - 0.5;
            ySrc = ((double)i + 0.5)/yScale - 0.5;
            if(xSrc < 0) xSrc = 0;
            if(ySrc < 0) ySrc = 0;
            if(xSrc > infoHeader.biWidth) xSrc = infoHeader.biWidth;
            if(ySrc > infoHeader.biHeight) ySrc = infoHeader.biHeight;
            src = getBiliner(xSrc, ySrc);
            imgDataCpy(src, (dstImgData + i*dstWidth + j));
        }
    }
    imgData = dstImgData;
    infoHeader.biHeight = dstHeight;
    infoHeader.biWidth = dstWidth;
    infoHeader.biSizeImage = dstHeight * (dstWidth + 4 - dstWidth % 4);
    fileHeader.bfSize = infoHeader.biSizeImage + fileHeader.bfOffBits;

}

void BMPIMG::medianFiltering()
{
    BYTE rList[9];
    BYTE bList[9];
    BYTE gList[9];
    IMAGEDATA pixList[9];
    int cnt = 0;
    IMAGEDATA *dstImgData = (IMAGEDATA*)malloc(sizeof(IMAGEDATA) * infoHeader.biWidth * infoHeader.biHeight);
    //1st line
    for(int i = 0; i< infoHeader.biWidth; i++){
        imgDataCpy((imgData + i), (dstImgData + i));
        cnt++;
    }
    for(int i = 1; i < infoHeader.biHeight-1; i++){
        //begin of the line
        imgDataCpy((imgData + cnt), (dstImgData + cnt));
        cnt++;
        for(int j = 1; j < infoHeader.biWidth-1; j++){
            pixList[0] = getPixelData(j-1, i-1);
            pixList[1] = getPixelData(j-1, i);
            pixList[2] = getPixelData(j-1, i+1);
            pixList[3] = getPixelData(j, i-1);
            pixList[4] = getPixelData(j, i);
            pixList[5] = getPixelData(j, i+1);
            pixList[6] = getPixelData(j+1, i-1);
            pixList[7] = getPixelData(j+1, i);
            pixList[8] = getPixelData(j+1, i+1);
            for(int k = 0; k < 9; k++){
                rList[k] = pixList[k].red;
                bList[k] = pixList[k].blue;
                gList[k] = pixList[k].green;
            }
            std::sort(bList, bList+9);
            (dstImgData + cnt)->blue = bList[4];
            if(infoHeader.biBitCount == 24){
                std::sort(rList, rList+9);
                std::sort(gList, gList+9);
                (dstImgData + cnt)->red = rList[4];
                (dstImgData + cnt)->green = gList[4];
            }
            cnt++;
        }
        //end of the line
        imgDataCpy((imgData + cnt), (dstImgData + cnt));
        cnt++;
    }
    //last line
    for(int i = 0; i< infoHeader.biWidth; i++){
        imgDataCpy((imgData + i), (dstImgData + i));
        cnt++;
    }
    free(imgData);
    imgData = dstImgData;
    return;
}

void BMPIMG::gaussianSmoothing(double mse, int blurRadius)
{
    IMAGEDATA *dstImgData = (IMAGEDATA*)malloc(sizeof(IMAGEDATA) * infoHeader.biWidth * infoHeader.biHeight);
    int cnt = 0;
    for(int i = 0; i < blurRadius; i++){
        for(int j = 0; j < infoHeader.biWidth; j++){
            imgDataCpy((imgData + cnt), (dstImgData + cnt));
            cnt++;
        }
    }
    for(int i = blurRadius; i < infoHeader.biHeight - blurRadius; i++){
        for(int j = 0; j < blurRadius; j++){
            imgDataCpy((imgData + cnt), (dstImgData + cnt));
            cnt++;
        }
        for(int j = blurRadius; j < infoHeader.biWidth - blurRadius; j++){
            imgDataCpy(getGaussianColor(j, i, blurRadius, mse), (dstImgData + cnt));
            cnt++;
        }
        for(int j = infoHeader.biWidth - blurRadius; j < infoHeader.biWidth; j++){
            imgDataCpy((imgData + cnt), (dstImgData + cnt));
            cnt++;
        }
    }
    for(int i = infoHeader.biHeight - blurRadius; i < infoHeader.biHeight; i++){
        for(int j = 0; j < infoHeader.biWidth; j++){
            imgDataCpy((imgData + cnt), (dstImgData + cnt));
            cnt++;
        }
    }
    free(imgData);
    imgData = dstImgData;
    return;
}
