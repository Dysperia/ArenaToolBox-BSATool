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
    /**
     * @brief Palette constructor. Extract a palette from the given data
     * @param rgbs palette description given as 256 RGB triplet (hence 768 length vector)
     * @param sixBitsColor colors are expected to be coded on 6 bits if true, 8 bits otherwise
     */
    explicit Palette(const QVector<char> &rgbs, bool sixBitsColor = true);

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
