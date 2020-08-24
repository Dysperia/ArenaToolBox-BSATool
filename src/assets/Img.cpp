#include <utility>
#include <utils/Compression.h>
#include <error/Status.h>
#include <log/Logger.h>

#include "Img.h"

using namespace std;

//******************************************************************************
// Constructors
//******************************************************************************
Img::Img(const QVector<char> &imgData, const Palette &palette) {
    QDataStream stream = QDataStream(QByteArray((imgData.constData()), imgData.size()));
    initFromStreamAndPalette(stream, palette);
}

Img::Img(QDataStream &imgData, const Palette &palette) {
    initFromStreamAndPalette(imgData, palette);
}

Img::Img(QVector<char> imgData, quint16 width, quint16 height, Palette palette) :
        mImageData(std::move(imgData)), mWidth(width), mHeight(height), mPalette(std::move(palette)) {
    mQImage = QImage(reinterpret_cast<uchar *>(mImageData.data()), mWidth, mHeight, mWidth, QImage::Format_Indexed8);
    mQImage.setColorTable(mPalette.getColorTable());
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

quint16 Img::dataSize() const {
    return mDataSize;
}

void Img::setDataSize(const quint16 &dataSize) {
    mDataSize = dataSize;
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

void Img::initFromStreamAndPalette(QDataStream &imgDataStream, const Palette &palette) {
    if (!isStreamAtLeastThisSize(imgDataStream, 12)) {
        Logger::getInstance().log(Logger::MessageType::ERROR, QStringLiteral("Image data is too short"));
        return;
    }
    imgDataStream.setByteOrder(QDataStream::LittleEndian);
    imgDataStream >> mOffsetX;
    imgDataStream >> mOffsetY;
    imgDataStream >> mWidth;
    imgDataStream >> mHeight;
    imgDataStream >> mCompressionFlag;
    imgDataStream >> mPaletteFlag;
    imgDataStream >> mDataSize;
    if (!isStreamAtLeastThisSize(imgDataStream, mDataSize)) {
        Logger::getInstance().log(Logger::MessageType::ERROR, QStringLiteral("Image data is too short"));
        return;
    }
    QVector<char> rawData(mCompressionFlag == 0x08 ? mDataSize - 2 : mDataSize);
    if (mCompressionFlag == 0x00) {
        if (readDataFromStream(imgDataStream, rawData, mDataSize)) {
            mImageData = rawData;
            validatePixelDataAndCreateImage();
        }
    } else if (mCompressionFlag == 0x02) {
        if (readDataFromStream(imgDataStream, rawData, mDataSize)) {
            try {
                mImageData = Compression::uncompressRLEByLine(rawData, mWidth, mHeight);
                validatePixelDataAndCreateImage();
            }
            catch (Status &e) {
                Logger::getInstance().log(Logger::MessageType::ERROR, e.message());
                return;
            }
        }
    } else if (mCompressionFlag == 0x04) {
        if (readDataFromStream(imgDataStream, rawData, mDataSize)) {
            try {
                mImageData = Compression::uncompressLZSS(rawData);
                validatePixelDataAndCreateImage();
            }
            catch (Status &e) {
                Logger::getInstance().log(Logger::MessageType::ERROR, e.message());
                return;
            }
        }
    } else if (mCompressionFlag == 0x08) {
        quint16 uncompressedSize = 0;
        imgDataStream >> uncompressedSize;
        if (readDataFromStream(imgDataStream, rawData, mDataSize - 2)) {
            try {
                mImageData = Compression::uncompressDeflate(rawData, uncompressedSize);
                validatePixelDataAndCreateImage();
            }
            catch (Status &e) {
                Logger::getInstance().log(Logger::MessageType::ERROR, e.message());
                return;
            }
        }
    } else {
        Logger::getInstance().log(Logger::MessageType::ERROR, QStringLiteral("This image compression is not supported : ") + QString::number(mCompressionFlag));
        return;
    }
    // palette setup. Integrated preferred if exists no matter the parameter
    if (!mQImage.isNull()) {
        bool streamLongEnough = isStreamAtLeastThisSize(imgDataStream, 768);
        if (mPaletteFlag & 1u && streamLongEnough) {
            QVector<char> paletteDescription(768);
            imgDataStream.readRawData(paletteDescription.data(), 768);
            mPalette = Palette(paletteDescription, true);
            mQImage.setColorTable(mPalette.getColorTable());
        } else {
            if (mPaletteFlag & 1u && !streamLongEnough) {
                Logger::getInstance().log(Logger::MessageType::ERROR, QStringLiteral("Integrated palette could not be read"));
            }
            mQImage.setColorTable(mPalette.getColorTable());
        }
    }
}

bool Img::readDataFromStream(QDataStream &imgDataStream, QVector<char> &rawData,
                             quint16 size) const {
    if (imgDataStream.readRawData(rawData.data(), size) < 0) {
        Logger::getInstance().log(Logger::MessageType::ERROR, QStringLiteral("Error while reading image data"));
        return false;
    }
    return true;
}
