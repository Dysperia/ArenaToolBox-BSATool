#ifndef BSATOOL_PALETTE_H
#define BSATOOL_PALETTE_H

#include <QRgb>
#include <QVector>

// TODO new class
class Palette
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Palette constructor. Build a default 256 colors gray palette
     * with colors from (R=1, G=1, B=1, A=255) to (R=255, G=255, B=255, A=255)
     */
    Palette();

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    QVector<QRgb> getColorTable() const;

    //**************************************************************************
    // Methods
    //**************************************************************************

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    QVector<QRgb> mColorTable{};
};

#endif // BSATOOL_PALETTE_H
