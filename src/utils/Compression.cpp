#include <c++/array>
#include "Compression.h"


QVector<char> Compression::uncompressLZSS(QVector<char> compressedData) {
    // init sliding window
    std::array<char, 4096> window{};
    quint16 windowCurrentPosition(0);
    for (; windowCurrentPosition < 0xFEE; ++windowCurrentPosition) {
        window[windowCurrentPosition] = 0x20;
    }
    // flags to know what to do of the 8 next uncompression operations
    quint8 flags(0);

    // uncompressing
    while (!compressedData.isEmpty()) {
        // next flag

    }
    return QVector<char>();
}

QVector<char> Compression::compressLZSS(const QVector<char> &uncompressData) {
    // TODO rewrite from original
    return QVector<char>();
}
