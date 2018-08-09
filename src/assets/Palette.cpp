#include "Palette.h"

Palette::Palette()
{
    for (int i=0; i<256; i++) {
        mColorTable.append(qRgb(i,i,i));
    }
}

QVector<QRgb> Palette::getColorTable() const
{
    return mColorTable;
}
