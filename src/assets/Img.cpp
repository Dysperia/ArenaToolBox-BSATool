#include "Img.h"

//******************************************************************************
// Constructors
//******************************************************************************
Img::Img(const QVector<char> &imgData)
{
    if (imgData.size() > 12) {
        QDataStream imgDataStream(QByteArray((imgData.constData()), imgData.size()));
        imgDataStream.setByteOrder(QDataStream::LittleEndian);
        imgDataStream >> mOffsetX;
        imgDataStream >> mOffsetY;
        imgDataStream >> mWidth;
        imgDataStream >> mHeight;
        imgDataStream >> mCompressionFlag;
        imgDataStream >> mPaletteFlag;
        imgDataStream >> mDataSize;
        if (imgData.size() >= mDataSize + 12) {
            QVector<uchar> compressedImageData(mDataSize);
            imgDataStream.readRawData(reinterpret_cast<char *>(compressedImageData.data()), mDataSize);
            mImageData = compressedImageData;
            mQImage = QImage(mImageData.data(), mWidth, mHeight, QImage::Format_Indexed8);
            mQImage.setColorTable(mPalette.getColorTable());
        }
    }
}

Img::Img(QVector<uchar> &imgData, quint16 width, quint16 height, Palette &palette):
    mWidth(width), mHeight(height), mPalette(palette)
{
    mImageData = imgData;
    mQImage = QImage(mImageData.data(), mWidth, mHeight, QImage::Format_Indexed8);
    mQImage.setColorTable(mPalette.getColorTable());
}

//******************************************************************************
// Getters/setters
//******************************************************************************
quint16 Img::offsetX() const
{
    return mOffsetX;
}

void Img::setOffsetX(const quint16 &offsetX)
{
    mOffsetX = offsetX;
}

quint16 Img::offsetY() const
{
    return mOffsetY;
}

void Img::setOffsetY(const quint16 &offsetY)
{
    mOffsetY = offsetY;
}

quint16 Img::width() const
{
    return mWidth;
}

void Img::setWidth(const quint16 &width)
{
    mWidth = width;
}

quint16 Img::height() const
{
    return mHeight;
}

void Img::setHeight(const quint16 &height)
{
    mHeight = height;
}

quint8 Img::compressionFlag() const
{
    return mCompressionFlag;
}

void Img::setCompressionFlag(const quint8 &compressionFlag)
{
    mCompressionFlag = compressionFlag;
}

quint8 Img::paletteFlag() const
{
    return mPaletteFlag;
}

void Img::setPaletteFlag(const quint8 &paletteFlag)
{
    mPaletteFlag = paletteFlag;
}

quint16 Img::dataSize() const
{
    return mDataSize;
}

void Img::setDataSize(const quint16 &dataSize)
{
    mDataSize = dataSize;
}

Palette Img::palette() const
{
    return mPalette;
}

void Img::setPalette(const Palette &palette)
{
    mPalette = palette;
}

QImage Img::qImage() const
{
    return mQImage;
}
