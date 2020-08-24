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
     * @brief test LZSS compression
     */
    void testLZSSCompression();
    /**
     * @brief test deflate uncompression
     */
    void testDeflateUncompression();
    void testDeflateUncompressionWithReset();
    /**
     * @brief test deflate compression
     */
    void testDeflateCompression();
    void testDeflateCompressionWithReset();
    /**
     * @brief test RLE by line uncompression
     */
    void testRLEByLineUncompression();
    /**
     * @brief test RLE by line compression
     */
    void testRLEByLineCompression();
    /**
     * @brief test encryption decryption
     */
    void testEncryptionDecryption();

private:
    [[nodiscard]] QVector<char> readFile(const QString &fileName) const;
};


#endif //BSATOOL_COMPRESSIONTEST_H
