#include <utils/Compression.h>
#include <error/Status.h>
#include <log/Logger.h>
#include <utils/BitsStreams.h>
#include <assets/Cfa.h>
#include <assets/Img.h>

//******************************************************************************
// Constructors
//******************************************************************************
Cfa::Cfa(const QVector<char> &data, Palette palette) : mPalette(std::move(palette)) {
    QDataStream stream = QDataStream(QByteArray((data.constData()), data.size()));
    initFromStreamAndPalette(stream, data.size());
}

//******************************************************************************
// Getters/setters
//******************************************************************************
quint16 Cfa::offsetX() const {
    return mOffsetX;
}

void Cfa::setOffsetX(const quint16 &offsetX) {
    mOffsetX = offsetX;
}

quint16 Cfa::offsetY() const {
    return mOffsetY;
}

void Cfa::setOffsetY(const quint16 &offsetY) {
    mOffsetY = offsetY;
}

quint16 Cfa::width() const {
    return mWidth;
}

void Cfa::setWidth(const quint16 &width) {
    mWidth = width;
}

quint16 Cfa::height() const {
    return mHeight;
}

void Cfa::setHeight(const quint16 &height) {
    mHeight = height;
}

Palette Cfa::palette() const {
    return mPalette;
}

void Cfa::setPalette(const Palette &palette) {
    mPalette = palette;
}

QVector<QImage> Cfa::qImages() const {
    return mQImages;
}

//******************************************************************************
// Methods
//******************************************************************************
void Cfa::initFromStreamAndPalette(QDataStream &dataStream, const quint16 &dataSize) {
    try {
        // reading header
        Img::verifyStream(dataStream, 14);
        dataStream.setByteOrder(QDataStream::LittleEndian);
        quint8 bitsPerPixel, frameNumber;
        quint16 compressedWidth, totalHeaderSize, totalFileSize;
        dataStream >> mWidth;
        dataStream >> mHeight;
        dataStream >> compressedWidth;
        dataStream >> mOffsetX;
        dataStream >> mOffsetY;
        dataStream >> bitsPerPixel;
        dataStream >> frameNumber;
        dataStream >> totalHeaderSize;
        Img::verifyStream(dataStream, totalHeaderSize - 14);
        QVector<quint16> frameDataOffsets;
        frameDataOffsets.push_back(totalHeaderSize);
        for (int frameIndex = 1; frameIndex < frameNumber; ++frameIndex) {
            quint16 frameDataOffset;
            dataStream >> frameDataOffset;
            frameDataOffsets.push_back(frameDataOffset);
        }
        dataStream.skipRawData((31 - frameNumber) * 2); // skipping remaining empty offset
        dataStream >> totalFileSize;
        if (totalFileSize == 0) {
            totalFileSize = dataSize;
        }
        frameDataOffsets.push_back(totalFileSize); // pushing virtual offset to compute size of last frame
        quint16 colorTableSize(totalHeaderSize - 76);
        QVector<char> colorTableRealIndexes(colorTableSize);
        dataStream.readRawData(colorTableRealIndexes.data(), colorTableSize);
        // reading frames
        quint8 leftOverBits = NB_BITS_IN_BYTE - bitsPerPixel;
        for (int frameIndex = 0; frameIndex < frameNumber; ++frameIndex) {
            // reading and uncompressing frame data
            quint16 compressedFrameDataSize = frameDataOffsets[frameIndex + 1] - frameDataOffsets[frameIndex];
            Img::verifyStream(dataStream, compressedFrameDataSize);
            QVector<char> compressedFrameData(compressedFrameDataSize);
            dataStream.readRawData(compressedFrameData.data(), compressedFrameDataSize);
            // RLE uncompression
            const QVector<char> frameData = Compression::uncompressRLE(compressedFrameData, compressedWidth * mHeight);
            QVector<char> frame;
            // Bits expansion for each line of pixels
            for (int lineIndex = 0; lineIndex < mHeight; ++lineIndex) {
                deque<char> compressedLine(compressedWidth);
                QVector<char>::const_iterator start = frameData.begin() + compressedWidth * lineIndex;
                std::copy(start, start + compressedWidth, compressedLine.begin());
                BitsReader bitsReader(compressedLine);
                for (int pixelIndex = 0; pixelIndex < mWidth; ++pixelIndex) {
                    quint8 pixel = bitsReader.getBits() >> leftOverBits;
                    bitsReader.removeBits(bitsPerPixel);
                    // if no color table (8 bits per pixel for example -> direct original pixel value)
                    if (colorTableSize == 0) {
                        frame.push_back(char(pixel));
                    }
                    else {
                        if (pixel >= colorTableRealIndexes.size()) {
                            throw Status(-1, QStringLiteral("pixel outside color table"));
                        }
                        frame.push_back(colorTableRealIndexes[pixel]);
                    }
                }
            }
            mFramesData.push_back(frame);
        }
        // verifying frames and building QImages
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