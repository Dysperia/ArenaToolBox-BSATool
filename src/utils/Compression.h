#ifndef BSATOOL_COMPRESSION_H
#define BSATOOL_COMPRESSION_H

#include <QtCore/QVector>

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
    static uchar getNextUnsignedByte(QVector<char> &compressedData);

    /**
     * Extract the front byte of the vector and remove it
     * from the vector
     * @param compressedData data from which extract the front byte
     * @return the next byte
     */
    static char getNextByte(QVector<char> &data);

public:
    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Uncompressed data with a LZSS algorithm
     * @param compressedData to uncompress
     * @return the uncompressed data
     */
    static QVector<char> uncompressLZSS(QVector<char> compressedData);

    /**
     * Compressed data with a LZSS algorithm
     * @param uncompressedData to compress
     * @return the compressed data
     */
    static QVector<char> compressLZSS(QVector<char> uncompressData);

    /**
     * Encrypt data according to the encryption key given. The same key is
     * used to encrypt and decrypt using a incrementing counter and xor operation
     * @param data to encrypt or decrypt
     * @param cryptKey encryption / decryption key
     */
    static QVector<char> encryptDecrypt(QVector<char> data, QVector<quint8> cryptKey = {0xEA, 0x7B, 0x4E, 0xBD, 0x19, 0xC9, 0x38, 0x99});
};

#endif // BSATOOL_COMPRESSION_H
