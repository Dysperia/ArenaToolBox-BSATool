#include <error/Status.h>
#include <deque>
#include "Compression.h"
#include "SlidingWindow.h"

//**************************************************************************
// Methods
//**************************************************************************
uchar Compression::getNextUnsignedByte(deque<char> &data) {
    char nextByte = getNextByte(data);
    return reinterpret_cast<uchar &>(nextByte);
}

char Compression::getNextByte(deque<char> &data) {
    if (data.empty()) {
        throw Status(-1, QStringLiteral("Unexpected end of data"));
    }
    const char &byte = data.front();
    data.pop_front();
    return byte;
}

QVector<char> Compression::uncompressLZSS(const QVector<char>& compressedData) {
    // deque to allow fast first element removal and random element access
    deque<char> compressDataDeque;
    for (const auto &byte : compressedData) {
        compressDataDeque.push_back(byte);
    }
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
    while (!compressDataDeque.empty()) {
        // shifting flags and getting next 8 if empty
        flags = flags >> 1u;
        if ((flags & 0xFF00u) == 0) {
            flags = getNextUnsignedByte(compressDataDeque) | 0xFF00u;
        }
        // need to insert next byte
        if ((flags & 0x01u) == 1) {
            char nextByte = getNextByte(compressDataDeque);
            uncompressedData.push_back(nextByte);
            // sliding window
            window.insert(nextByte);
        }
        // need to copy sequence from window
        else {
            quint8 byte1 = getNextUnsignedByte(compressDataDeque);
            quint8 byte2 = getNextUnsignedByte(compressDataDeque);
            quint8 length = (byte2 & 0x0Fu) + 3;
            quint16 startIndex = ((byte2 & 0xF0u) << 4u) | byte1;
            // copying sequence
            for (int offset = 0; offset < length; ++offset) {
                char uncompressByte = window.readAtIndex(startIndex + offset);
                uncompressedData.push_back(uncompressByte);
                // sliding window
                window.insert(uncompressByte);
            }
        }
    }
    return uncompressedData;
}

QVector<char> Compression::compressLZSS(const QVector<char>& uncompressData) {
    // deque to allow fast first element removal and random element access
    deque<char> uncompressDataDeque;
    for (const auto &byte : uncompressData) {
        uncompressDataDeque.push_back(byte);
    }
    // Max possible length for a duplicate
    // cannot be higher than 18 because length-3 should take at most 4 bits
    const quint8 max_duplicate_length(18);
    // init sliding window of maximum size since offset are encoded using at most 12 bits
    SlidingWindow<char, 4096> window;
    for (int i(0); i < 0xFEE; ++i) {
        window.insert(0x20);
    }
    // compression buffer
    QVector<char> compressedBytesBuffer;
    // how many flags have been used
    int flagsNumber(0);
    // flags to know what are the 8 next operations
    // Higher bits are used to know how many flags are remaining
    // Lower bits indicate a sequence copy from window if 0, only the next byte if 1
    quint8 flags(0);
    // compressedData
    QVector<char> compressedData;
    while (!uncompressDataDeque.empty()) {
        // if flags full, need to write buffer
        if (flagsNumber == 8) {
            // Writing flags, then buffer
            compressedData.push_back(flags);
            flags = 0;
            flagsNumber = 0;
            for (char i : compressedBytesBuffer) {
                compressedData.push_back(i);
            }
            compressedBytesBuffer.clear();
        }
        // length of duplicate
        quint8 length(0);
        // start index of duplicate in window
        quint16 startIndex(0);
        const char &nextUncompressedByte = uncompressDataDeque.front();
        // searching for an ongoing duplicate using the possibly rewritten part of the window
        // longest possible considering max duplicate length and remaining uncompressed data
        quint8 max_possible_duplicate_length(uncompressDataDeque.size() < max_duplicate_length ? uncompressDataDeque.size() : max_duplicate_length);
        QVector<char> snapshotFutureWindow;
        for (int i(-max_duplicate_length); i < 0; ++i) {
            snapshotFutureWindow.push_back(window.readAtIndex(window.getMCurrentInsertPosition() + i));
        }
        for (size_t i(0); i < max_possible_duplicate_length - 1; ++i) {
            snapshotFutureWindow.push_back(uncompressDataDeque[i]);
        }
        for (int i(0); i < max_duplicate_length; ++i) {
            if (nextUncompressedByte == snapshotFutureWindow[i]) {
                // computing sequence length
                quint8 tempLength(1);
                // computing while not the longest length and available data
                while (tempLength < max_possible_duplicate_length &&
                       snapshotFutureWindow[i + tempLength] == uncompressDataDeque[tempLength]) {
                    tempLength++;
                }
                if (tempLength >= length) {
                    length = tempLength;
                    startIndex = window.getStandardEquivalentIndex(window.getMCurrentInsertPosition() + i - max_duplicate_length);
                }
            }
        }
        // Search through buffer in case there is a longest duplicate to copy avoiding the possibly
        // rewritten section already search before
        if (length < max_duplicate_length) {
            quint8 tempLength(0);
            quint16 tempStartIndex(0);
            // searching a first byte match until longest found or all window searched
            // starting at 1 to avoid the window current index to be safe with any uncompression algorithm
            for (int i = 1; i < 4096 - max_duplicate_length && length < max_duplicate_length; ++i) {
                tempStartIndex = window.getStandardEquivalentIndex(window.getMCurrentInsertPosition() + i);
                // Found a possible match
                if (nextUncompressedByte == window.readAtIndex(tempStartIndex)) {
                    tempLength = 1;
                    // computing length for the found match, while checking if enough data available for it
                    while (tempLength < uncompressDataDeque.size() &&
                           tempLength < max_duplicate_length &&
                           uncompressDataDeque[tempLength] == window.readAtIndex(tempStartIndex + tempLength)) {
                        tempLength++;
                    }
                    // keeping only if longer than a previous one
                    if (tempLength >= length) {
                        length = tempLength;
                        startIndex = tempStartIndex;
                    }
                }
            }
        }
        // Writing compressed data to buffer
        if (length > 2) {
            // next flag is 0
            flags = flags >> 1u;
            flagsNumber++;
            // encoding 4 bits length and 12 bits offset
            uint8_t byte1 = startIndex & 0x00FFu;
            uint8_t byte2 = ((startIndex & 0x0F00u) >> 4u) | (length - 3u);
            // writing coordinates to copy
            compressedBytesBuffer.push_back(reinterpret_cast<char &>(byte1));
            compressedBytesBuffer.push_back(reinterpret_cast<char &>(byte2));
            // sliding window
            for (int i(0); i < length; i++) {
                window.insert(uncompressDataDeque.front());
                uncompressDataDeque.pop_front();
            }
        } else {
            // next flag is 1
            flags = flags >> 1u;
            flags |= 0x80u;
            flagsNumber++;
            // writing next byte to copy
            const char &nextUncompressByte = uncompressDataDeque.front();
            uncompressDataDeque.pop_front();
            compressedBytesBuffer.push_back(nextUncompressByte);
            // sliding window
            window.insert(nextUncompressByte);
        }
    }
    // If less than 8 operations because end of file, need to flush the remaining buffer
    if (flagsNumber > 0) {
        flags = flags >> (8u - flagsNumber);
        compressedData.push_back(reinterpret_cast<char &>(flags));
        for (char i : compressedBytesBuffer) {
            compressedData.push_back(i);
        }
    }
    return compressedData;
}

QVector<char> Compression::encryptDecrypt(const QVector<char>& data, QVector<quint8> cryptKey) {
    // deque to allow fast first element removal and random element access
    deque<char> dataDeque;
    for (const auto &byte : data) {
        dataDeque.push_back(byte);
    }
    // encryption cycles through cryptKey
    int cryptKeyIndex(0);
    // counter resets after 256 operations
    quint8 counter(0);
    // output
    QVector<char> cryptData;
    cryptData.reserve(dataDeque.size());
    // encryption / decryption process
    while (!dataDeque.empty()) {
        // real key to XOR against
        quint8 effectiveKey = counter + cryptKey[cryptKeyIndex];
        // pushing new encrypted / decrypted byte
        cryptData.push_back(getNextUnsignedByte(dataDeque) ^ effectiveKey);
        // preparing next step
        counter++;
        cryptKeyIndex = (cryptKeyIndex + 1) % cryptKey.size();
    }
    return cryptData;
}
