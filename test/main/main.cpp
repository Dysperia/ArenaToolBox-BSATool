#include <QtTest/QTest>
#include <QTextCodec>
#include "../bsa/BsaFileTest.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    BsaFileTest bsaFileTest;

    return QTest::qExec(&bsaFileTest, argc, argv)/* |
            QTest::qExec(&testSuite2, argc, argv)*/;
}
