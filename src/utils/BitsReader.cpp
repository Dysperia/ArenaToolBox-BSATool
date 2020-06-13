#include <error/Status.h>
#include "BitsReader.h"

//**************************************************************************
// Class Methods
//**************************************************************************
uchar BitsReader::getNextUnsignedByte(deque<char> &data) {
    char nextByte = getNextByte(data);
    return reinterpret_cast<uchar &>(nextByte);
}

char BitsReader::getNextByte(deque<char> &data) {
    if (data.empty()) {
        throw Status(-1, QStringLiteral("Unexpected end of data"));
    }
    const char &byte = data.front();
    data.pop_front();
    return byte;
}

//**************************************************************************
// Constructors
//**************************************************************************
BitsReader::BitsReader(deque<char> &source): mSource(source) {}

//**************************************************************************
// Getters/setters
//**************************************************************************
quint8 BitsReader::getRemainingBits() const {
    return mRemainingBits;
}

quint16 BitsReader::getBits() {
    // if needed, getting 8 new bits to ensure having at least 8 usable
    while (mRemainingBits <= 0x08) {
        quint16 nextBits;
        if (!mSource.empty()) {
            nextBits = getNextUnsignedByte(mSource) & 0x00FFu;
        } else {
            nextBits = 0x0000u;
        }
        quint8 nbGarbageBitsInHighByte = 0x08u - mRemainingBits;
        quint16 movedNewBits = nextBits << nbGarbageBitsInHighByte;
        mBits = movedNewBits | mBits;
        mRemainingBits += 8;
    }
    return mBits;
}

//**************************************************************************
// Methods
//**************************************************************************
void BitsReader::removeBits(const quint8 &nbBitsToRemove) {
    quint8 minNb = min(mRemainingBits, nbBitsToRemove);
    mBits = mBits << minNb;
    mRemainingBits -= minNb;
}
