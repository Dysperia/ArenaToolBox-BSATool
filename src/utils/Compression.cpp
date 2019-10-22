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
    unsigned char decompressedData[64000];
    for (int j = 0; j < uncompressData.size(); ++j) {
        decompressedData[j] = uncompressData.at(j);
    }
    unsigned char compressedData[64000];
    uint16_t decompressedLengh(64000);
    unsigned char *compBuffer = new unsigned char[4096];
    for (int i(0); i<0xFEE; i++)
    {
        compBuffer[i] = 0x20;
    }
    uint16_t decompDataPosition(0), compBufferPosition(0x0FEE), compDataPosition(0);
    uint16_t bufferPos(0), bufferPosToCopy(0);
    uint8_t bitmask(0), counter(0), tempCounter(0);
    std::vector<uint8_t> tempData;
    int tempDataNb(0);
    while (decompDataPosition < decompressedLengh)
    {
        // if tempData full: 8 operations done
        if (tempDataNb == 8)
        {
            // Writting data to compressed data
            compressedData[compDataPosition] = bitmask;
            compDataPosition ++;
            for (size_t i(0); i<tempData.size(); i++)
            {
                compressedData[compDataPosition] = tempData[i];
                compDataPosition ++;
            }
            bitmask = 0;
            tempData.clear();
            tempDataNb = 0;
        }
        counter = 0;
        bufferPosToCopy = 0;
        // if serie of same value and buffer has value at pos - 1 -> hot copy
        if (decompressedData[decompDataPosition] == compBuffer[(compBufferPosition - 1) & 0x0FFF])
        {
            tempCounter = 0;
            while (decompressedData[decompDataPosition] == decompressedData[decompDataPosition + tempCounter] &&
                   tempCounter < 18 &&
                   decompDataPosition + tempCounter < decompressedLengh)
            {
                tempCounter ++;
            }
            if (tempCounter > counter)
            {
                counter = tempCounter;
                bufferPosToCopy = (compBufferPosition - 1) & 0x0FFF;
            }
        }
        // Search through buffer if needed
        if (counter < 18) // optimisation: if not found the longest to copy
        {
            bufferPos = (compBufferPosition + 0x12) & 0x0FFF;
            while (bufferPos != compBufferPosition && counter < 18)
            {
                if (decompressedData[decompDataPosition] == compBuffer[bufferPos])
                {
                    tempCounter = 0;
                    while (decompressedData[decompDataPosition + tempCounter] == compBuffer[(bufferPos + tempCounter) & 0x0FFF] &&
                           tempCounter < 18 &&
                           decompDataPosition + tempCounter < decompressedLengh &&
                           ((bufferPos + tempCounter) & 0x0FFF) != compBufferPosition)
                    {
                        tempCounter ++;
                    }
                    if (tempCounter > counter)
                    {
                        counter = tempCounter;
                        bufferPosToCopy = bufferPos;
                    }
                }
                bufferPos = (bufferPos + 1) & 0x0FFF;
            }
        }
        // Writting tempData
        if (counter > 2)
        {
            bitmask = bitmask >> 1;
            tempDataNb ++;
            uint8_t byte1 = bufferPosToCopy & 0x00FF;
            uint8_t byte2 = ((bufferPosToCopy & 0x0F00) >> 4) | (counter - 3);
            tempData.push_back(byte1);
            tempData.push_back(byte2);
            for (int i(0); i<counter; i++)
            {
                compBuffer[compBufferPosition] = decompressedData[decompDataPosition];
                decompDataPosition ++;
                compBufferPosition = (compBufferPosition + 1) & 0x0FFF;
            }
        }
        else
        {
            bitmask = (bitmask >> 1) | 0x80;
            tempDataNb ++;
            tempData.push_back(decompressedData[decompDataPosition]);
            compBuffer[compBufferPosition] = decompressedData[decompDataPosition];
            decompDataPosition ++;
            compBufferPosition = (compBufferPosition + 1) & 0x0FFF;
        }
    }
    // If less than 8 operations because end of file
    if (tempDataNb > 0)
    {
        bitmask = bitmask >> (8-tempDataNb);
        // Writting data to compressed data
        compressedData[compDataPosition] = bitmask;
        compDataPosition ++;
        for (size_t i(0); i<tempData.size(); i++)
        {
            compressedData[compDataPosition] = tempData[i];
            compDataPosition ++;
        }
        bitmask = 0;
        tempData.clear();
        tempDataNb = 0;
    }
    delete[] compBuffer;

    QVector<char> comp;
    for (int k = 0; k < compDataPosition; ++k) {
        comp.push_back(compressedData[k]);
    }

    return comp;

}
//QVector<char> Compression::compressLZSS(QVector<char> uncompressData) {
//    // init sliding window
//    SlidingWindow<char, 4096> window;
//    for (int i(0); i < 0xFEE; ++i) {
//        window.insert(0x20);
//    }
//    // compression buffer
//    QVector<char> compressedBytesBuffer;
//    // how many flags have been used
//    int flagsNumber(0);
//    // flags to know what are the 8 next operations
//    // Higher bits are used to know how many flags are remaining
//    // Lower bits indicate a sequence copy from window if 0, only the next byte if 1
//    quint8 flags(0);
//    // uncompressedData
//    QVector<char> compressedData;
//    while (!uncompressData.isEmpty()) {
//        // if flags full, need to write buffer
//        if (flagsNumber == 8) {
//            // Writing flags, then buffer
//            compressedData.push_back(flags);
//            for (size_t i(0); i < compressedBytesBuffer.size(); i++) {
//                compressedData.push_back(compressedBytesBuffer.takeFirst());
//            }
//            flags = 0;
//            flagsNumber = 0;
//        }
//        quint8 length(0);
//        quint16 startIndex(0);
//        // if sequence of same value and buffer has value at pos - 1 -> hot copy
//        char &nextUncompressedByte = uncompressData.front();
//        if (nextUncompressedByte == window.readAtIndex(window.getMCurrentInsertPosition() - 1)) {
//            quint8 tempLength(0);
//            while (tempLength < uncompressData.size() && tempLength < 18 && nextUncompressedByte == uncompressData.at(tempLength)) {
//                tempLength ++;
//            }
//            if (tempLength > length) {
//                length = tempLength;
//                startIndex = (window.getMCurrentInsertPosition() - 1) % window.getWindow().size();
//            }
//        }
//        // Search through buffer in case there is a longest match to copy
//        if (length < 18) {
//            quint8 tempLength(0);
//            quint16 tempStartIndex(0);
//            // since the decompression process read from the window before update it
//            // we can search for the longest match anywhere including at current insertion index
//            for (tempStartIndex = 0; tempStartIndex < window.getWindow().size() && length < 18; ++tempStartIndex) {
//                if (nextUncompressedByte == window.readAtIndex(tempStartIndex)) {
//                    tempLength = 1;
//                    while (tempLength < uncompressData.size() && tempLength < 18 && uncompressData.at(tempLength) == window.readAtIndex(tempStartIndex + tempLength)) {
//                        tempLength ++;
//                    }
//                    if (tempLength > length) {
//                        length = tempLength;
//                        startIndex = tempStartIndex;
//                    }
//                }
//            }
//        }
//        // Writting compressed data to buffer
//        if (length > 2) {
//            // next flag is 0
//            flags = flags >> 1u;
//            flagsNumber ++;
//            uint8_t byte1 = startIndex & 0x00FFu;
//            uint8_t byte2 = ((startIndex & 0x0F00u) >> 4u) | (length - 3u);
//            compressedBytesBuffer.push_back(byte1);
//            compressedBytesBuffer.push_back(byte2);
//            for (int i(0); i < length; i++) {
//                window.insert(uncompressData.takeFirst());
//            }
//        }
//        else {
//            // next flag is 1
//            flags = flags >> 1u;
//            flags |= 0x80u;
//            flagsNumber ++;
//            char nextUncompressByte = uncompressData.takeFirst();
//            compressedBytesBuffer.push_back(nextUncompressByte);
//            window.insert(nextUncompressByte);
//        }
//    }
//    // If less than 8 operations because end of file, need to flush the buffer anyway
//    if (flagsNumber > 0) {
//        flags = flags >> (8u - flagsNumber);
//        // Writing buffer to compressed data
//        compressedData.push_back(flags);
//        for (size_t i(0); i < compressedBytesBuffer.size(); i++) {
//            compressedData.push_back(compressedBytesBuffer.takeFirst());
//        }
//    }
//    return compressedData;
//}

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
