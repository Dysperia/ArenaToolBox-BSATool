#include <log/Logger.h>
#include <utils/Compression.h>
#include "Dfa.h"
#include "Img.h"

using namespace std;

//******************************************************************************
// Constructors
//******************************************************************************
Dfa::Dfa(const QVector<char> &data, Palette palette) : mPalette(std::move(palette)) {
    QDataStream stream = QDataStream(QByteArray((data.constData()), data.size()));
    initFromStreamAndPalette(stream);
}

//******************************************************************************
// Getters/setters
//******************************************************************************
quint16 Dfa::offsetX() const {
    return mOffsetX;
}

void Dfa::setOffsetX(const quint16 &offsetX) {
    mOffsetX = offsetX;
}

quint16 Dfa::offsetY() const {
    return mOffsetY;
}

void Dfa::setOffsetY(const quint16 &offsetY) {
    mOffsetY = offsetY;
}

quint16 Dfa::width() const {
    return mWidth;
}

void Dfa::setWidth(const quint16 &width) {
    mWidth = width;
}

quint16 Dfa::height() const {
    return mHeight;
}

void Dfa::setHeight(const quint16 &height) {
    mHeight = height;
}

Palette Dfa::palette() const {
    return mPalette;
}

void Dfa::setPalette(const Palette &palette) {
    mPalette = palette;
}

QVector<QImage> Dfa::qImages() const {
    return mQImages;
}

//******************************************************************************
// Methods
//******************************************************************************
void Dfa::initFromStreamAndPalette(QDataStream &dataStream) {
    try {
        // reading header
        Img::verifyStream(dataStream, 12);
        dataStream.setByteOrder(QDataStream::LittleEndian);
        quint16 frameCount, firstFrameDataSize;
        dataStream >> frameCount;
        dataStream >> mOffsetX;
        dataStream >> mOffsetY;
        dataStream >> mWidth;
        dataStream >> mHeight;
        // reading first frame
        dataStream >> firstFrameDataSize;
        QVector<char> firstFrameCompressedData(firstFrameDataSize);
        Img::verifyStream(dataStream, firstFrameDataSize);
        dataStream.readRawData(firstFrameCompressedData.data(), firstFrameDataSize);
        const QVector<char> &firstFrameData = Compression::uncompressRLE(firstFrameCompressedData, mWidth * mHeight);
        mFramesData.push_back(firstFrameData);
        // reading other frame
        for (int frameIndex = 1; frameIndex < frameCount; ++frameIndex) {
            mFramesData.push_back(QVector<char>(firstFrameData.size()));
            copy(firstFrameData.begin(), firstFrameData.end(), mFramesData.last().begin());
        }
        for (int frameIndex = 1; frameIndex < frameCount; ++frameIndex) {
            Img::verifyStream(dataStream, 2);
            quint16 differentialSize, chunkCount;
            dataStream >> differentialSize;
            Img::verifyStream(dataStream, differentialSize);
            dataStream >> chunkCount;
            auto &frame = mFramesData[frameIndex];
            // reading frame chunks
            for (int chunkIndex = 0; chunkIndex < chunkCount; ++chunkIndex) {
                quint16 offset, pixelNumber;
                dataStream >> offset >> pixelNumber;
                // using pixels of this chunk
                for (int pixelIndex = 0; pixelIndex < pixelNumber; ++pixelIndex) {
                    dataStream.readRawData(frame.data() + offset + pixelIndex, 1);
                }
            }
        }
        for (auto &frame : mFramesData) {
            if (mWidth * mHeight != frame.size()) {
                throw Status(-1, QStringLiteral("This image of the animation contained too much or too few pixels for its size"));
            }
            mQImages.push_back(QImage(reinterpret_cast<uchar *>(frame.data()), mWidth, mHeight, mWidth, QImage::Format_Indexed8));
            mQImages.last().setColorTable(mPalette.getColorTable());
        }
    }
    catch (Status &e) {
        Logger::getInstance().log(Logger::MessageType::ERROR, e.message());
        return;
    }
}