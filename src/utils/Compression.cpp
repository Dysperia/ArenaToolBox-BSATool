#include <error/Status.h>
#include <deque>
#include "Compression.h"
#include "SlidingWindow.h"
#include "HuffmanTree.h"
#include "BitsReader.h"

//**************************************************************************
// Methods
//**************************************************************************
QVector<char> Compression::uncompressLZSS(const QVector<char> &compressedData) {
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
            flags = BitsReader::getNextUnsignedByte(compressDataDeque) | 0xFF00u;
        }
        // need to insert next byte
        if ((flags & 0x01u) == 1) {
            char nextByte = BitsReader::getNextByte(compressDataDeque);
            uncompressedData.push_back(nextByte);
            // sliding window
            window.insert(nextByte);
        }
            // need to copy sequence from window
        else {
            quint8 byte1 = BitsReader::getNextUnsignedByte(compressDataDeque);
            quint8 byte2 = BitsReader::getNextUnsignedByte(compressDataDeque);
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

QVector<char> Compression::compressLZSS(const QVector<char> &uncompressData) {
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
        // search for a duplicate
        const DuplicateSearchResult &duplicate = searchDuplicateInSlidingWindow(uncompressDataDeque,
                                                                                max_duplicate_length, window);
        // Writing compressed data to buffer
        if (duplicate.length > 2) {
            // next flag is 0
            flags = flags >> 1u;
            flagsNumber++;
            // encoding 4 bits length and 12 bits offset
            uint8_t byte1 = duplicate.startIndex & 0x00FFu;
            uint8_t byte2 = ((duplicate.startIndex & 0x0F00u) >> 4u) | (duplicate.length - 3u);
            // writing coordinates to copy
            compressedBytesBuffer.push_back(reinterpret_cast<char &>(byte1));
            compressedBytesBuffer.push_back(reinterpret_cast<char &>(byte2));
            // sliding window
            for (int i(0); i < duplicate.length; i++) {
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

QVector<char> Compression::uncompressDeflate(const QVector<char> &compressedData) {
    quint8 offsetHighBits[256] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
            0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
            0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
            0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
            0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
            0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
            0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F,
            0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23, 0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27,
            0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F,
            0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F
    };

    quint8 nbBitsMissingInOffsetLowBits[256] = {
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
            0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
            0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
            0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
            0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08
    };

    // init huffman tree
    HuffmanTree huffmanTree = HuffmanTree();
    // deque to allow fast first element removal and random element access
    deque<char> compressDataDeque;
    for (const auto &byte : compressedData) {
        compressDataDeque.push_back(byte);
    }
    // init sliding window
    SlidingWindow<char, 4096> window;
    for (int i(0); i < 4036; ++i) {
        window.insert(0x20);
    }
    // uncompressed data
    QVector<char> uncompressedData;
    // bits reader to manage reading of incoming bits from compressed data
    BitsReader bitsReader(compressDataDeque);
    // decompressing data from source
    while (!compressDataDeque.empty()) {
        // searching leaf value in tree from input. The real value is the leaf value minus 627
        quint16 colorOrNbToCopy = huffmanTree.findLeaf(bitsReader) - 627;
        // single byte copy
        if (colorOrNbToCopy < 256) {
            quint8 colorByte = colorOrNbToCopy & 0x00FFu;
            uncompressedData.push_back(colorByte);
            window.insert(colorByte);
        }
        // copy string from window
        else {
            // Reading index for offset tables
            quint16 offsetTableIdx = bitsReader.getBits() >> 8u;
            bitsReader.removeBits(8);
            // init offset low bits
            quint16 offsetToCopyLowBits = offsetTableIdx;
            // init offset high bits
            quint16 offsetToCopyHighBits = (offsetHighBits[offsetTableIdx] & 0x00FFu) << 6u;
            // getting missing bits and added them if needed to get the full offset low bits
            quint16 nbBitsToReadAndAdd = (nbBitsMissingInOffsetLowBits[offsetTableIdx] & 0x00FFu) - 2u;
            for (; nbBitsToReadAndAdd > 0; nbBitsToReadAndAdd--) {
                quint16 bits = bitsReader.getBits();
                bitsReader.removeBits(1);
                offsetToCopyLowBits = (offsetToCopyLowBits << 1u) + (bits >> 15u);
            }
            // gettting offset from high and low bits
            quint16 offsetFromCurrentPosition = (offsetToCopyLowBits & 0x003Fu) | offsetToCopyHighBits;
            // string start position in window
            quint16 copyPosition = (window.getMCurrentInsertPosition() - offsetFromCurrentPosition - 1) & 0x0FFFu;
            // getting length from leaf value (minus 256 because 256 color leaves before length leaves)
            // the length value stored in leaves is the length - 3
            quint16 nbToCopy = colorOrNbToCopy - 256 + 3;
            // copy
            for (quint16 i(0); i < nbToCopy; i++) {
                quint8 colorByte = window.readAtIndex(copyPosition + i);
                uncompressedData.push_back(colorByte);
                window.insert(colorByte);
            }
        }
    }
    return uncompressedData;
}

Compression::DuplicateSearchResult Compression::searchDuplicateInSlidingWindowLookAheadOnly(
        const deque<char> &uncompressDataDeque, const quint8 max_duplicate_length,
        const SlidingWindow<char, 4096> &window) {
    // search longest possible considering max duplicate length and remaining uncompressed data
    quint8 max_possible_duplicate_length(
            uncompressDataDeque.size() < max_duplicate_length ? uncompressDataDeque.size() : max_duplicate_length);
    // building preview window using current data and future one
    QVector<char> snapshotFutureWindow;
    // end of current buffer
    for (int i(-max_duplicate_length); i < 0; ++i) {
        snapshotFutureWindow.push_back(window.readAtIndex(window.getMCurrentInsertPosition() + i));
    }
    // data that will next be written in buffer
    for (size_t i(0); i < max_possible_duplicate_length - 1; ++i) {
        snapshotFutureWindow.push_back(uncompressDataDeque[i]);
    }
    // searching for duplicate
    DuplicateSearchResult result = {0, 0};
    const char &nextUncompressedByte = uncompressDataDeque.front();
    for (int i(0); i < max_duplicate_length && result.length < max_possible_duplicate_length; ++i) {
        // found start for a match
        if (nextUncompressedByte == snapshotFutureWindow[i]) {
            // computing sequence length
            quint8 tempLength(1);
            // computing while not the longest length and available data
            while (tempLength < max_possible_duplicate_length &&
                   snapshotFutureWindow[i + tempLength] == uncompressDataDeque[tempLength]) {
                tempLength++;
            }
            // writing result if longer
            if (tempLength > result.length) {
                result.length = tempLength;
                result.startIndex = window.getStandardEquivalentIndex(
                        window.getMCurrentInsertPosition() + i - max_duplicate_length);
            }
        }
    }
    return result;
}

Compression::DuplicateSearchResult Compression::searchDuplicateInSlidingWindowNoLookAhead(
        const deque<char> &uncompressDataDeque, const quint8 max_duplicate_length,
        const SlidingWindow<char, 4096> &window) {
    DuplicateSearchResult result = {0, 0};
    quint8 tempLength(0);
    quint16 tempStartIndex(0);
    // searching a first byte match until longest found or all window searched
    // starting at offset 1 from current position to avoid the window current index
    const char &nextUncompressedByte = uncompressDataDeque.front();
    for (int i = 1; i < 4096 - max_duplicate_length && result.length < max_duplicate_length; ++i) {
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
            if (tempLength > result.length) {
                result.length = tempLength;
                result.startIndex = tempStartIndex;
            }
        }
    }
    return result;
}

Compression::DuplicateSearchResult Compression::searchDuplicateInSlidingWindow(const deque<char> &uncompressDataDeque,
                                                                               const quint8 max_duplicate_length,
                                                                               const SlidingWindow<char, 4096> &window) {
    // searching for an ongoing duplicate using the possibly rewritten part of the window
    const DuplicateSearchResult &lookAhead = searchDuplicateInSlidingWindowLookAheadOnly(uncompressDataDeque,
                                                                                         max_duplicate_length,
                                                                                         window);
    DuplicateSearchResult noLookAhead = {0, 0};
    // not longest found
    if (lookAhead.length < max_duplicate_length) {
        // Search through buffer in case there is a longest duplicate to copy avoiding the possibly
        // rewritten section already search before
        noLookAhead = searchDuplicateInSlidingWindowNoLookAhead(uncompressDataDeque,
                                                                max_duplicate_length,
                                                                window);
    }
    return lookAhead.length > noLookAhead.length ? lookAhead : noLookAhead;
}

QVector<char> Compression::encryptDecrypt(const QVector<char> &data, QVector<quint8> cryptKey) {
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
        cryptData.push_back(BitsReader::getNextUnsignedByte(dataDeque) ^ effectiveKey);
        // preparing next step
        counter++;
        cryptKeyIndex = (cryptKeyIndex + 1) % cryptKey.size();
    }
    return cryptData;
}
