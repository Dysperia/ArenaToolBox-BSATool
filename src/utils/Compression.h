#ifndef BSATOOL_COMPRESSION_H
#define BSATOOL_COMPRESSION_H

#include <QtCore/QVector>
#include <deque>
#include "SlidingWindow.h"

using namespace std;

/**
 * Compression class contains various helpers used to compress and uncompress
 * the data. It also contains a helper to encrypt/decrypt INF file
 */
class Compression
{
private:
    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Extract the front byte of the vector as an unsigned byte and remove it
     * from the vector
     * @param compressedData data from which extract the front byte
     * @return the next unsigned byte
     */
    static uchar getNextUnsignedByte(deque<char> &compressedData);

    /**
     * Extract the front byte of the vector and remove it
     * from the vector
     * @param compressedData data from which extract the front byte
     * @return the next byte
     */
    static char getNextByte(deque<char> &data);

    /**
     * Search for a duplicate using the possibly soon rewritten part of the sliding window
     * @param uncompressDataDeque data from which read the ongoing data to compress
     * @param max_duplicate_length max length for a duplicate to copy
     * @param window sliding window
     * @param length of the duplicate. The variable is set only if a longest duplicate is found
     * @param startIndex of the duplicate. The variable is set only if a longest duplicate is found
     */
    static void compressLZSS_searchForHotCopy(const deque<char> &uncompressDataDeque, quint8 max_duplicate_length,
            const SlidingWindow<char, 4096> &window, quint8 &length, quint16 &startIndex);

    /**
     * Search for a duplicate in the sliding window, avoiding the last max_duplicate_length of the window
     * @param uncompressDataDeque data from which read the ongoing data to compress
     * @param max_duplicate_length max length for a duplicate to copy
     * @param window sliding window
     * @param length of the duplicate. The variable is set only if a longest duplicate is found
     * @param startIndex of the duplicate. The variable is set only if a longest duplicate is found
     */
    static void compressLZSS_searchDuplicateInWindow(const deque<char> &uncompressDataDeque,
            quint8 max_duplicate_length, const SlidingWindow<char, 4096> &window, quint8 & length, quint16 & startIndex);

public:
    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Uncompressed data with a LZSS algorithm
     * @param compressedData to uncompress
     * @return the uncompressed data
     */
    static QVector<char> uncompressLZSS(const QVector<char>& compressedData);

    /**
     * Compressed data with a LZSS algorithm
     * @param uncompressedData to compress
     * @return the compressed data
     */
    static QVector<char> compressLZSS(const QVector<char>& uncompressData);

    /**
     * Encrypt data according to the encryption key given. The same key is
     * used to encrypt and decrypt using a incrementing counter and xor operation
     * @param data to encrypt or decrypt
     * @param cryptKey encryption / decryption key
     */
    static QVector<char> encryptDecrypt(const QVector<char>& data, QVector<quint8> cryptKey = {0xEA, 0x7B, 0x4E, 0xBD, 0x19, 0xC9, 0x38, 0x99});
};

#endif // BSATOOL_COMPRESSION_H
