#include "Palette.h"

Palette::Palette()
{

}

QVector<QRgb> Palette::getColorTable() const
{
    return colorTable;
}

void Palette::setColorTable(const QVector<QRgb> &value)
{
    colorTable = value;
}

QVector<QRgb> Palette::getColorTable() const
{
    return colorTable;
}

void Palette::setColorTable(const QVector<QRgb> &value)
{
    colorTable = value;
}
