#include <QtTest/QtTest>
#include "CompressionTest.h"
#include "../../src/utils/Compression.h"

void CompressionTest::testLZSSUncompression() {
    QWARN("Should uncompress the file and get the original data");
    QVector<char> uncompressedDataFromFile = readFile(QStringLiteral("ressources/uncompressedLZSS.data"));
    QVERIFY(!uncompressedDataFromFile.isEmpty());
    QVector<char> compressedData = readFile(QStringLiteral("ressources/compressedLZSS.data"));
    QVERIFY(!compressedData.isEmpty());
    QVector<char> uncompressedDataFromAlgorithm = Compression::uncompressLZSS(compressedData);
    QVERIFY(!uncompressedDataFromAlgorithm.isEmpty());
    QCOMPARE(uncompressedDataFromAlgorithm == uncompressedDataFromFile, true);
}

QVector<char> CompressionTest::readFile(const QString &fileName) const {
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);
    QVector<char> data(file.size());
    stream.readRawData(data.data(), file.size());
    file.close();
    return data;
}
