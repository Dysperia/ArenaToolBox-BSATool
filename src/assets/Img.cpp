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
        if (imgData.size() >= mDataSize + 12 && mCompressionFlag == 0x00) {
            QVector<char> compressedImageData(mDataSize);
            imgDataStream.readRawData(compressedImageData.data(), mDataSize);
            mImageData = compressedImageData;
            validatePixelDataAndCreateImage();
        } else if (mCompressionFlag == 0x04) {
            QVector<char> compressedImageData(mDataSize);
            imgDataStream.readRawData(compressedImageData.data(), mDataSize);
            try {
                mImageData = Compression::uncompressLZSS(compressedImageData);
                validatePixelDataAndCreateImage();
            }
            catch (Status &e) {
                Logger::getInstance().log(Logger::MessageType::ERROR, e.message());
            }
        } else if (mCompressionFlag == 0x08) {
            QVector<char> compressedImageData(mDataSize - 2);
            quint16 uncompressedSize = 0;
            imgDataStream >> uncompressedSize;
            imgDataStream.readRawData(compressedImageData.data(), mDataSize - 2);
            try {
                mImageData = Compression::uncompressDeflate(compressedImageData, uncompressedSize);
                validatePixelDataAndCreateImage();
            }
            catch (Status &e) {
                Logger::getInstance().log(Logger::MessageType::ERROR, e.message());
            }
        } else {
            Logger::getInstance().log(Logger::MessageType::ERROR, QStringLiteral("This image is not supported"));
            return;
        }
        // palette setup. Integrated preferred if exists no matter the parameter
        if (!mQImage.isNull()) {
            if (mPaletteFlag == 1) {
                QVector<char> paletteDescription(768);
                imgDataStream.readRawData(paletteDescription.data(), 768);
                mPalette = Palette(paletteDescription, true);
                mQImage.setColorTable(mPalette.getColorTable());
            } else {
                mQImage.setColorTable(mPalette.getColorTable());
            }
        }
    }
}

Img::Img(QVector<char> imgData, quint16 width, quint16 height, Palette palette) :
        mImageData(std::move(imgData)), mWidth(width), mHeight(height), mPalette(std::move(palette)) {
    mQImage = QImage(reinterpret_cast<uchar *>(mImageData.data()), mWidth, mHeight, mWidth, QImage::Format_Indexed8);
    mQImage.setColorTable(mPalette.getColorTable());
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
