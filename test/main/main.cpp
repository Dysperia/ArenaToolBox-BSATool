#include <QtTest/QTest>
#include <QTextCodec>
#include <utils/CompressionTest.h>
#include <bsa/BsaFileTest.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    BsaFileTest bsaFileTest;
    CompressionTest compressionTest;

    return QTest::qExec(&bsaFileTest, argc, argv) |
            QTest::qExec(&compressionTest, argc, argv);
}
#pragma clang diagnostic pop
