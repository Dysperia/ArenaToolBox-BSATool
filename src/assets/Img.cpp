#include "Img.h"

Img::Img(QVector<char> &imgData)
{

}

Img::Img(QVector<char> &imgData, quint16 width, quint16 height, Palette palette)
{

}

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
