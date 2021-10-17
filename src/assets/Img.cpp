#include <utility>
#include <utils/Compression.h>
#include <error/Status.h>
#include <log/Logger.h>

#include <assets/Img.h>

using namespace std;

//******************************************************************************
// Constructors
//******************************************************************************
Img::Img(const QVector<char> &imgData, Palette palette) {
    QDataStream stream = QDataStream(QByteArray((imgData.constData()), imgData.size()));
    initFromStreamAndPalette(stream, std::move(palette));
}

Img::Img(QDataStream &imgData, Palette palette) {
    initFromStreamAndPalette(imgData, std::move(palette));
}

Img::Img(const QVector<char> &imgData, quint16 width, quint16 height, Palette palette) :
        mWidth(width), mHeight(height), mRawDataSize(width * height) {
    QDataStream stream = QDataStream(QByteArray((imgData.constData()), imgData.size()));
    initFromStreamAndPalette(stream, std::move(palette), true);
}

Img::Img(QDataStream &imgData, quint16 width, quint16 height, Palette palette) :
        mWidth(width), mHeight(height), mRawDataSize(width * height) {
    initFromStreamAndPalette(imgData, std::move(palette), true);
}

//******************************************************************************
// Getters/setters
//******************************************************************************
quint16 Img::offsetX() const {
    return mOffsetX;
}

void Img::setOffsetX(const quint16 &offsetX) {
    mOffsetX = offsetX;
}

quint16 Img::offsetY() const {
    return mOffsetY;
}

void Img::setOffsetY(const quint16 &offsetY) {
    mOffsetY = offsetY;
}

quint16 Img::width() const {
    return mWidth;
}

void Img::setWidth(const quint16 &width) {
    mWidth = width;
}

quint16 Img::height() const {
    return mHeight;
}

void Img::setHeight(const quint16 &height) {
    mHeight = height;
}

quint8 Img::compressionFlag() const {
    return mCompressionFlag;
}

void Img::setCompressionFlag(const quint8 &compressionFlag) {
    mCompressionFlag = compressionFlag;
}

quint8 Img::paletteFlag() const {
    return mPaletteFlag;
}

void Img::setPaletteFlag(const quint8 &paletteFlag) {
    mPaletteFlag = paletteFlag;
}

Palette Img::palette() const {
    return mPalette;
}

void Img::setPalette(const Palette &palette) {
    mPalette = palette;
}

QImage Img::qImage() const {
    return mQImage;
}

//******************************************************************************
// Methods
//******************************************************************************
void Img::validatePixelDataAndCreateImage() {
    if (mWidth * mHeight != mImageData.size()) {
        mQImage = QImage();
        Logger::getInstance().log(Logger::MessageType::ERROR,
                                  QStringLiteral("This image contained too much or too few pixels for its size"));
    } else {
        mQImage = QImage(reinterpret_cast<uchar *>(mImageData.data()), mWidth, mHeight, mWidth,
                         QImage::Format_Indexed8);
    }
}

bool Img::isStreamAtLeastThisSize(QDataStream &stream, int byteNumber) {
    if (stream.atEnd() || stream.status() != QDataStream::Status::Ok) {
        return false;
    }
    stream.startTransaction();
    int read = stream.skipRawData(byteNumber);
    stream.rollbackTransaction();
    stream.resetStatus();
    return read == byteNumber;
}

void Img::verifyStream(QDataStream &dataStream, int size) {
    if (!isStreamAtLeastThisSize(dataStream, size)) {
        throw Status(-1, QStringLiteral("Image data is too short"));
    }
}

bool Img::readDataFromStream(QDataStream &imgDataStream, QVector<char> &rawData,
                             quint16 size) {
    if (imgDataStream.readRawData(rawData.data(), size) < 0) {
        Logger::getInstance().log(Logger::MessageType::ERROR, QStringLiteral("Error while reading image data"));
        return false;
    }
    return true;
}

void Img::initFromStreamAndPalette(QDataStream &imgDataStream, Palette palette, bool noHeader) {
    try {
        if (!noHeader) {
            verifyStream(imgDataStream, 12);
            imgDataStream.setByteOrder(QDataStream::LittleEndian);
            imgDataStream >> mOffsetX;
            imgDataStream >> mOffsetY;
            imgDataStream >> mWidth;
            imgDataStream >> mHeight;
            imgDataStream >> mCompressionFlag;
            imgDataStream >> mPaletteFlag;
            imgDataStream >> mRawDataSize;
        }
        verifyStream(imgDataStream, mRawDataSize);
        QVector<char> rawData(mCompressionFlag == 0x08 ? mRawDataSize - 2 : mRawDataSize);
        if (mCompressionFlag == 0x00) {
            readDataFromStream(imgDataStream, rawData, mRawDataSize);
            mImageData = rawData;
            validatePixelDataAndCreateImage();
        } else if (mCompressionFlag == 0x02) {
            readDataFromStream(imgDataStream, rawData, mRawDataSize);
            mImageData = Compression::uncompressRLEByLine(rawData, mWidth, mHeight);
            validatePixelDataAndCreateImage();
        } else if (mCompressionFlag == 0x04) {
            readDataFromStream(imgDataStream, rawData, mRawDataSize);
            mImageData = Compression::uncompressLZSS(rawData);
            validatePixelDataAndCreateImage();
        } else if (mCompressionFlag == 0x08) {
            quint16 uncompressedSize = 0;
            imgDataStream >> uncompressedSize;
            readDataFromStream(imgDataStream, rawData, mRawDataSize - 2);
            mImageData = Compression::uncompressDeflate(rawData, uncompressedSize);
            validatePixelDataAndCreateImage();
        } else {
            throw Status(-1, QStringLiteral("This image compression is not supported : ") +
                             QString::number(mCompressionFlag));
        }
    }
    catch (Status &e) {
        Logger::getInstance().log(Logger::MessageType::ERROR, e.message());
        return;
    }
    // palette setup. Integrated preferred if exists
    if (!mQImage.isNull()) {
        bool paletteIntegrated = mPaletteFlag & 1u;
        if (paletteIntegrated) {
            bool streamLongEnough = isStreamAtLeastThisSize(imgDataStream, 768);
            if (streamLongEnough) {
                QVector<char> paletteDescription(768);
                imgDataStream.readRawData(paletteDescription.data(), 768);
                mPalette = Palette(paletteDescription, true);
                mQImage.setColorTable(mPalette.getColorTable());
            } else {
                Logger::getInstance().log(Logger::MessageType::ERROR,
                                          QStringLiteral("Integrated palette could not be read"));
                mQImage.setColorTable(mPalette.getColorTable());
            }
        } else {
            mPalette = std::move(palette);
            mQImage.setColorTable(mPalette.getColorTable());
        }
    }
}
