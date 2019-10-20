#ifndef BSATOOL_COMPRESSIONTEST_H
#define BSATOOL_COMPRESSIONTEST_H

#include <QObject>

class CompressionTest  : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief test LZSS uncompression
     */
    void testLZSSUncompression();
    /**
     * @brief test encryption decryption
     */
    void testEncryptionDecryption();

private:
    QVector<char> readFile(const QString &fileName) const;
};


#endif //BSATOOL_COMPRESSIONTEST_H
