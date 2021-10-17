#ifndef BSATOOL_IMG_H
#define BSATOOL_IMG_H

#include <QImage>
#include <assets/Palette.h>

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
class Img
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief constructor of an empty invalid IMG (null QImage)
     */
    Img() = default;
    /**
     * @brief constructor of IMG with parsing of the header. The well
     * initialization can be checked with the potentially Null status of the
     * QImage
     * @param imgData data of the IMG file
     * @param palette palette used to display the IMG
     */
    explicit Img(const QVector<char> &imgData, Palette palette = Palette());
    /**
     * @brief constructor of IMG with parsing of the header. The well
     * initialization can be checked with the potentially Null status of the
     * QImage
     * @param imgData data of the IMG file. The stream will advance
     * @param palette palette used to display the IMG
     */
    explicit Img(QDataStream &imgData, Palette palette = Palette());
    /**
     * @brief constructor of IMG without parsing of the header. The well
     * initialization can be checked with the potentially Null status of the
     * QImage
     * @param imgData data of the IMG file
     * @param width of the IMG file
     * @param height of the IMG file
     * @param palette palette used to display the IMG
     */
    Img(const QVector<char> &imgData, quint16 width, quint16 height, Palette palette = Palette());
    /**
     * @brief constructor of IMG without parsing of the header. The well
     * initialization can be checked with the potentially Null status of the
     * QImage
     * @param imgData data of the IMG file. The stream will advance
     * @param width of the IMG file
     * @param height of the IMG file
     * @param palette palette used to display the IMG
     */
    Img(QDataStream &imgData, quint16 width, quint16 height, Palette palette = Palette());

    //**************************************************************************
    // Static Methods
    //**************************************************************************
    /**
     * Validate the length of the stream
     * @param stream to check
     * @return true if the stream is at least this size
     */
    static bool isStreamAtLeastThisSize(QDataStream &stream, int byteNumber);
    /**
     * Validate the length of the stream and throw a Status instance if not long enough
     * @param stream to check
     * @throw Status if not long enough
     */
    static void verifyStream(QDataStream &stream, int byteNumber);

    /**
     * Read data from stream and log an error if the operation is not a success
     * @param imgDataStream stream to read
     * @param rawData data destination
     * @param size byte number to read
     * @return true if success, false otherwise
     */
    static bool readDataFromStream(QDataStream &imgDataStream, QVector<char> &rawData, quint16 size);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    [[nodiscard]] quint16 offsetX() const;
    void setOffsetX(const quint16 &offsetX);

    [[nodiscard]] quint16 offsetY() const;
    void setOffsetY(const quint16 &offsetY);

    [[nodiscard]] quint16 width() const;
    void setWidth(const quint16 &width);

    [[nodiscard]] quint16 height() const;
    void setHeight(const quint16 &height);

    [[nodiscard]] quint8 compressionFlag() const;
    void setCompressionFlag(const quint8 &compressionFlag);

    [[nodiscard]] quint8 paletteFlag() const;
    void setPaletteFlag(const quint8 &paletteFlag);

    [[nodiscard]] Palette palette() const;
    void setPalette(const Palette &palette);

    [[nodiscard]] QImage qImage() const;

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief offset X used to draw the image at the correct position on screen
     */
    quint16 mOffsetX{0};
    /**
     * @brief offset Y used to draw the image at the correct position on screen
     */
    quint16 mOffsetY{0};
    /**
     * @brief width of the image
     */
    quint16 mWidth{0};
    /**
     * @brief height of the image
     */
    quint16 mHeight{0};
    /**
     * @brief compression flag
     */
    quint8 mCompressionFlag{0};
    /**
     * @brief palette flag
     */
    quint8 mPaletteFlag{0};
    /**
     * @brief size of raw imgData (before uncompression)
     */
    quint16 mRawDataSize{0};
    /**
     * @brief color palette
     */
    Palette mPalette;
    /**
     * @brief image data
     */
    QVector<char> mImageData{};
    /**
     * @brief QImage version of this img, mainly used for display
     */
    QImage mQImage;

    //******************************************************************************
    // Methods
    //******************************************************************************
    /**
     * Validate image data by comparing pixel number and image size (height * width). If validation passed, a non null
     * QImage is created
     */
    void validatePixelDataAndCreateImage();

    /**
     * Init image from the given stream and palette. If no header,
     * mOffsetX, mOffsetY, mWidth, mHeight, mCompressionFlag, mPaletteFlag, mRawDataSize
     * should be set before calling this function
     * @param stream containing image data. The stream will advance
     * @param palette color table to use
     * @param noHeader true if data has no header to read
     */
    void initFromStreamAndPalette(QDataStream &imgDataStream, Palette palette, bool noHeader = false);
};

#endif // BSATOOL_IMG_H
