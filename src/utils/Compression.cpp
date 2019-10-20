#include <error/Status.h>
#include "Compression.h"
#include "SlidingWindow.h"

//**************************************************************************
// Methods
//**************************************************************************
uchar Compression::getNextUnsignedByte(QVector<char> &data) {
    char nextByte = getNextByte(data);
    return reinterpret_cast<uchar&>(nextByte);
}

char Compression::getNextByte(QVector<char> &data) {
    if (data.isEmpty()) {
        throw Status(-1, QStringLiteral("Unexpected end of data"));
    }
    return data.takeFirst();
}

QVector<char> Compression::uncompressLZSS(QVector<char> compressedData) {
    // init sliding window
    SlidingWindow<char, 4096> window;
    for (int i(0); i < 0xFEE; ++i) {
        window.insert(0x20);
    }
    // flags to know what are the 8 next operations
    // Higher bits are used to know how many flags are remaining
    // Lower bits indicate a sequence copy from window if 0, only the next byte if 1
    quint16 flags(0);
    // uncompressedData
    QVector<char> uncompressedData;
    try {
        // uncompressing
        while (!compressedData.isEmpty()) {
            // shifting flags and getting next 8 if empty
            flags = flags >> 1u;
            if ((flags & 0xFF00u) == 0) {
                flags = getNextUnsignedByte(compressedData) | 0xFF00u;
            }
            // insert next byte
            if ((flags & 0x01u) == 1) {
                char nextByte = getNextByte(compressedData);
                uncompressedData.push_back(nextByte);
                window.insert(nextByte);
            }
                // copy sequence from window
            else {
                quint8 byte1 = getNextUnsignedByte(compressedData);
                quint8 byte2 = getNextUnsignedByte(compressedData);
                quint8 length = (byte2 & 0x0Fu) + 3;
                quint16 startIndex = ((byte2 & 0xF0u) << 4u) | byte1;
                for (int offset = 0; offset < length; ++offset) {
                    char uncompressByte = window.readAtIndex(startIndex + offset);
                    uncompressedData.push_back(uncompressByte);
                    window.insert(uncompressByte);
                }
            }
        }
        return uncompressedData;
    }
    catch (Status &e) {
        return uncompressedData;
    }
}

QVector<char> Compression::compressLZSS(QVector<char> uncompressData) {
    return QVector<char>();
}

QVector<char> Compression::encryptDecrypt(QVector<char> data, QVector<quint8> cryptKey) {
    int cryptKeyIndex(0);
    quint8 counter(0);
    QVector<char> cryptData;
    cryptData.reserve(data.size());
    while (!data.isEmpty()) {
        quint8 effectiveKey = counter + cryptKey[cryptKeyIndex];
        cryptData.push_back(getNextUnsignedByte(data) ^ effectiveKey);
        counter ++;
        cryptKeyIndex = (cryptKeyIndex + 1) % cryptKey.size();
    }
    return cryptData;
}
