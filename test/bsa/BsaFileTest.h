#ifndef BSATOOL_BSAFILETEST_H
#define BSATOOL_BSAFILETEST_H

#include <QObject>

class BsaFileTest : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief test operator == and !=
     */
    void testEquality();
};

#endif // BSATOOL_BSAFILETEST_H
