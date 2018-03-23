#ifndef PALETTE_H
#define PALETTE_H

#include <QRgb>

// TODO new class
class Palette
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    Palette();

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    QVector<QRgb> getColorTable() const;

private:
    QVector<QRgb> colorTable;
};

#endif // PALETTE_H
