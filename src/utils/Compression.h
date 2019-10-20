#ifndef BSATOOL_COMPRESSION_H
#define BSATOOL_COMPRESSION_H

#include <QtCore/QVector>

// TODO new class
class Compression
{
public:

    static QVector<char> uncompressLZSS(QVector<char> compressedData);

    static QVector<char> compressLZSS(const QVector<char> &uncompressData);
};

#endif // BSATOOL_COMPRESSION_H
