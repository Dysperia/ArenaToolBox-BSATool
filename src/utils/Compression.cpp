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
    // Lower bits indicate a sequence copy from window if 0, copy the next incoming byte if 1
    quint16 flags(0);
    // uncompressed data
    QVector<char> uncompressedData;
    // uncompression process
    while (!compressedData.isEmpty()) {
        // shifting flags and getting next 8 if empty
        flags = flags >> 1u;
        if ((flags & 0xFF00u) == 0) {
            flags = getNextUnsignedByte(compressedData) | 0xFF00u;
        }
        // need to insert next byte
        if ((flags & 0x01u) == 1) {
            char nextByte = getNextByte(compressedData);
            uncompressedData.push_back(nextByte);
            // shifting sliding window
            window.insert(nextByte);
        }
        // need to copy sequence from window
        else {
            quint8 byte1 = getNextUnsignedByte(compressedData);
            quint8 byte2 = getNextUnsignedByte(compressedData);
            quint8 length = (byte2 & 0x0Fu) + 3;
            quint16 startIndex = ((byte2 & 0xF0u) << 4u) | byte1;
            // copying sequence
            for (int offset = 0; offset < length; ++offset) {
                char uncompressByte = window.readAtIndex(startIndex + offset);
                uncompressedData.push_back(uncompressByte);
                // shifting sliding window
                window.insert(uncompressByte);
            }
        }
    }
    return uncompressedData;
}

QVector<char> Compression::compressLZSS(QVector<char> uncompressData) {
    return QVector<char>();
}

QVector<char> Compression::encryptDecrypt(QVector<char> data, QVector<quint8> cryptKey) {
    // encryption cycles through cryptKey
    int cryptKeyIndex(0);
    // counter resets after 256 operations
    quint8 counter(0);
    // output
    QVector<char> cryptData;
    cryptData.reserve(data.size());
    // encryption / decryption process
    while (!data.isEmpty()) {
        // real key to XOR against
        quint8 effectiveKey = counter + cryptKey[cryptKeyIndex];
        // pushing new encrypted / decrypted byte
        cryptData.push_back(getNextUnsignedByte(data) ^ effectiveKey);
        // preparing next step
        counter ++;
        cryptKeyIndex = (cryptKeyIndex + 1) % cryptKey.size();
    }
    return cryptData;
}
