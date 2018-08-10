#ifndef BSAFILETEST_H
#define BSAFILETEST_H

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

#endif // BSAFILETEST_H
