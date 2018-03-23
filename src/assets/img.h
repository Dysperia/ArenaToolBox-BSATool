#ifndef IMG_H
#define IMG_H

#include <QImage>
#include "Palette.h"

/**
 * @brief Describe the IMG image format
 *
 * If the image has an header it follows the next structure
 * - Offset X : 2 bytes
 * - Offset Y : 2 bytes
 * - Width : 2 bytes
 * - Height : 2 bytes
 * - Compression flags : 1 byte
 * - Palette flags : 1 byte
 * - Image data size : 2 bytes
 * Then follows the image data and after the palette data if any
 */
class IMG
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief constructor of IMG with parsing of the header
     * @param imgData data of the IMG file
     */
    IMG(QVector<char> &imgData);
    /**
     * @brief constructor of IMG without parsing of the header
     */
    IMG(QVector<char> &imgData, quint16 width, quint16 height, Palette palette);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    quint16 offsetX() const;

    quint16 offsetY() const;

    quint16 width() const;
    void setWidth(const quint16 &width);

    quint16 height() const;
    void setHeight(const quint16 &height);

    quint8 compressionFlag() const;

    quint8 paletteFlag() const;

    quint16 dataSize() const;

    Palette palette() const;
    void setPalette(const Palette &palette);

    //**************************************************************************
    // Methods
    //**************************************************************************

private:
    /**
     * @brief offset X used to draw the image at the correct position on screen
     */
    quint16 mOffsetX;
    /**
     * @brief offset Y used to draw the image at the correct position on screen
     */
    quint16 mOffsetY;
    /**
     * @brief width of the image
     */
    quint16 mWidth;
    /**
     * @brief height of the image
     */
    quint16 mHeight;
    /**
     * @brief compression flag
     */
    quint8 mCompressionFlag;
    /**
     * @brief palette flag
     */
    quint8 mPaletteFlag;
    /**
     * @brief size of the image data
     */
    quint16 mDataSize;
    /**
     * @brief color palette
     */
    Palette mPalette;
};

#endif // IMG_H
