#include "BsaFileTest.h"
#include "../../src/bsa/BsaFile.h"
#include <QtTest/QTest>

void BsaFileTest::testEquality()
{
    QWARN("Should return equality for files not updated with same sizes, same offsets, same names and same index");
    BsaFile bsaFile1(10, 20, QString("file1"), 1);
    BsaFile bsaFile2(10, 20, QString("file1"), 1);
    QCOMPARE(bsaFile1 == bsaFile2, true);
    QCOMPARE(bsaFile1 != bsaFile2, false);

    QCOMPARE(BsaFile(0, 0, "INVALID", 0) == BsaFile::INVALID_BSAFILE, true);
    QCOMPARE(BsaFile(0, 0, "INVALID", 0) != BsaFile::INVALID_BSAFILE, false);

    QWARN("Should return equality for files not updated with same sizes, different offsets, same names and same index");
    bsaFile2 = BsaFile(10, 200, "file1", 1);
    QCOMPARE(bsaFile1 == bsaFile2, true);
    QCOMPARE(bsaFile1 != bsaFile2, false);

    QWARN("Should return equality for files not updated with different sizes, same offsets, same names and same index");
    bsaFile2 = BsaFile(100, 20, QString("file1"), 1);
    QCOMPARE(bsaFile1 == bsaFile2, true);
    QCOMPARE(bsaFile1 != bsaFile2, false);

    QWARN("Should return not equality for files not updated with same sizes, same offsets, different names and same index");
    bsaFile2 = BsaFile(10, 20, QString("file10"), 1);
    QCOMPARE(bsaFile1 == bsaFile2, false);
    QCOMPARE(bsaFile1 != bsaFile2, true);

    QWARN("Should return equality for files not updated with same sizes, same offsets, same names and different index");
    bsaFile2 = BsaFile(10, 20, QString("file1"), 10);
    QCOMPARE(bsaFile1 == bsaFile2, true);
    QCOMPARE(bsaFile1 != bsaFile2, false);

    QWARN("Should return equality for same files, but one updated with same updated sizes");
    bsaFile2 = BsaFile(10, 20, QString("file1"), 1);
    bsaFile2.setUpdated(true);
    bsaFile2.setUpdateFileSize(10);
    QCOMPARE(bsaFile1 == bsaFile2, true);
    QCOMPARE(bsaFile1 != bsaFile2, false);

    QWARN("Should return equality for same files, but one updated with different updated sizes");
    bsaFile2 = BsaFile(10, 20, QString("file1"), 1);
    bsaFile2.setUpdated(true);
    bsaFile2.setUpdateFileSize(100);
    QCOMPARE(bsaFile1 == bsaFile2, true);
    QCOMPARE(bsaFile1 != bsaFile2, false);

    QWARN("Should return equality for same files, both updated with same updated sizes");
    bsaFile1 = BsaFile(10, 20, QString("file1"), 1);
    bsaFile1.setUpdated(true);
    bsaFile1.setUpdateFileSize(100);
    bsaFile2 = BsaFile(10, 20, QString("file1"), 1);
    bsaFile2.setUpdated(true);
    bsaFile2.setUpdateFileSize(100);
    QCOMPARE(bsaFile1 == bsaFile2, true);
    QCOMPARE(bsaFile1 != bsaFile2, false);

    QWARN("Should return not equality for same files, both updated but with different updated sizes");
    bsaFile1 = BsaFile(10, 20, QString("file1"), 1);
    bsaFile1.setUpdated(true);
    bsaFile1.setUpdateFileSize(100);
    bsaFile2 = BsaFile(10, 20, QString("file1"), 1);
    bsaFile2.setUpdated(true);
    bsaFile2.setUpdateFileSize(1000);
    QCOMPARE(bsaFile1 == bsaFile2, true);
    QCOMPARE(bsaFile1 != bsaFile2, false);
}
