#include "img.h"

IMG::IMG(QVector<char> &imgData)
{

}

IMG::IMG(QVector<char> &imgData, quint16 width, quint16 height, Palette palette)
{

}

quint16 IMG::offsetX() const
{
    return mOffsetX;
}

void IMG::setOffsetX(const quint16 &offsetX)
{
    mOffsetX = offsetX;
}

quint16 IMG::offsetY() const
{
    return mOffsetY;
}

void IMG::setOffsetY(const quint16 &offsetY)
{
    mOffsetY = offsetY;
}

quint16 IMG::width() const
{
    return mWidth;
}

void IMG::setWidth(const quint16 &width)
{
    mWidth = width;
}

quint16 IMG::height() const
{
    return mHeight;
}

void IMG::setHeight(const quint16 &height)
{
    mHeight = height;
}

quint8 IMG::compressionFlag() const
{
    return mCompressionFlag;
}

void IMG::setCompressionFlag(const quint8 &compressionFlag)
{
    mCompressionFlag = compressionFlag;
}

quint8 IMG::paletteFlag() const
{
    return mPaletteFlag;
}

void IMG::setPaletteFlag(const quint8 &paletteFlag)
{
    mPaletteFlag = paletteFlag;
}

quint16 IMG::dataSize() const
{
    return mDataSize;
}

void IMG::setDataSize(const quint16 &dataSize)
{
    mDataSize = dataSize;
}

Palette IMG::palette() const
{
    return mPalette;
}

void IMG::setPalette(const Palette &palette)
{
    mPalette = palette;
}
