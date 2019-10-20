#ifndef BSATOOL_COMPRESSION_H
#define BSATOOL_COMPRESSION_H

#include <QtCore/QVector>

// TODO new class
class Compression
{
private:
    static uchar getNextUnsignedByte(QVector<char> &compressedData);

    static char getNextByte(QVector<char> &data);

public:

    static QVector<char> uncompressLZSS(QVector<char> compressedData);

    static QVector<char> compressLZSS(QVector<char> uncompressData);
};

#endif // BSATOOL_COMPRESSION_H
