#include <QtTest/QTest>
#include <QTextCodec>
#include <utils/CompressionTest.h>
#include <bsa/BsaFileTest.h>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    BsaFileTest bsaFileTest;
    CompressionTest compressionTest;

    return QTest::qExec(&bsaFileTest, argc, argv) |
            QTest::qExec(&compressionTest, argc, argv);
}
