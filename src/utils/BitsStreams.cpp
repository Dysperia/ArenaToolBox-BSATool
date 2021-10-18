#include <error/Status.h>
#include <utils/BitsStreams.h>

//**************************************************************************
// Statics
//**************************************************************************
uchar BitsReader::getNextUnsignedByte(deque<char> &data) {
    return uchar(getNextByte(data));
}

char BitsReader::getNextByte(deque<char> &data) {
    if (data.empty()) {
        throw Status(-1, QStringLiteral("Unexpected end of data"));
    }
    // copy value before freeing memory
    char byte = data.front();
    data.pop_front();
    return byte;
}

//**************************************************************************
// Constructors
//**************************************************************************
BitsReader::BitsReader(deque<char> &source): mSource(source) {}

BitsWriter::BitsWriter(QVector<char> &destination): mDestination(destination) {}

//**************************************************************************
// Getters/setters
//**************************************************************************
quint8 BitsReader::getRemainingBits() const {
    return mBitsBuffer.mRemainingBits;
}

quint8 BitsReader::getBits() {
    // if needed, getting 8 new bits to ensure having at least 8 usable
    while (mBitsBuffer.mRemainingBits <= NB_BITS_IN_BYTE) {
        quint16 nextBits;
        if (!mSource.empty()) {
            nextBits = getNextUnsignedByte(mSource) & 0x00FFu;
        } else {
            nextBits = 0x0000u;
        }
        quint8 nbGarbageBitsInHighByte = NB_BITS_IN_BYTE - mBitsBuffer.mRemainingBits;
        quint16 movedNewBits = nextBits << nbGarbageBitsInHighByte;
        mBitsBuffer.mBits = movedNewBits | mBitsBuffer.mBits;
        mBitsBuffer.mRemainingBits += 8;
    }
    return mBitsBuffer.mBits >> NB_BITS_IN_BYTE;
}

//**************************************************************************
// Methods
//**************************************************************************
void BitsReader::removeBits(const quint8 &nbBitsToRemove) {
    quint8 minNb = min(mBitsBuffer.mRemainingBits, nbBitsToRemove);
    mBitsBuffer.mBits = mBitsBuffer.mBits << minNb;
    mBitsBuffer.mRemainingBits -= minNb;
}

void BitsWriter::addBits(const quint8 byte, const quint8 nbBits) {
    // if needed, flushing bits to ensure having less than 8 usable
    while (mBitsBuffer.mRemainingBits >= NB_BITS_IN_BYTE) {
        flushHighByte();
    }
    quint8 nbGarbageBitsInHighByte = NB_BITS_IN_BYTE - mBitsBuffer.mRemainingBits;
    quint16 movedNewBits = byte << nbGarbageBitsInHighByte;
    mBitsBuffer.mBits = movedNewBits | mBitsBuffer.mBits;
    mBitsBuffer.mRemainingBits += nbBits;
}

void BitsWriter::flush() {
    while (mBitsBuffer.mRemainingBits > 0) {
        flushHighByte();
    }
}

void BitsWriter::flushHighByte() {
    if (mBitsBuffer.mRemainingBits > 0) {
        quint8 highByte = mBitsBuffer.mBits >> NB_BITS_IN_BYTE;
        mDestination.push_back(reinterpret_cast<char &>(highByte));
        mBitsBuffer.mBits <<= NB_BITS_IN_BYTE;
        mBitsBuffer.mRemainingBits -= min(mBitsBuffer.mRemainingBits, NB_BITS_IN_BYTE);
    }
}
