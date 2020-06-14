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
class Compression {
public:
    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Uncompressed data with a LZSS algorithm
     * @param compressedData to uncompress
     * @return the uncompressed data
     */
    static QVector<char> uncompressLZSS(const QVector<char> &compressedData);

    /**
     * Compressed data with a LZSS algorithm
     * @param uncompressedData to compress
     * @return the compressed data
     */
    static QVector<char> compressLZSS(const QVector<char> &uncompressData);

    /**
     * Uncompressed data with a deflate algorithm
     * @param compressedData to uncompress
     * @return the uncompressed data
     */
    static QVector<char> uncompressDeflate(const QVector<char> &compressedData);

    /**
     * Encrypt data according to the encryption key given. The same key is
     * used to encrypt and decrypt using a incrementing counter and xor operation
     * @param data to encrypt or decrypt
     * @param cryptKey encryption / decryption key
     */
    static QVector<char> encryptDecrypt(const QVector<char> &data,
                                        QVector<quint8> cryptKey = {0xEA, 0x7B, 0x4E, 0xBD, 0x19, 0xC9, 0x38, 0x99});
};

#endif // BSATOOL_COMPRESSION_H
