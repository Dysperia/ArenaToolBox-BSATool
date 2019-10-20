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

private:
    QVector<char> readFile(const QString &fileName) const;
};


#endif //BSATOOL_COMPRESSIONTEST_H
