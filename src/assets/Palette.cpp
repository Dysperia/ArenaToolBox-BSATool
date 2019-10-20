#include <QtCore/QDataStream>
#include <error/Status.h>
#include "Palette.h"

Palette::Palette()
{
    for (int i=0; i<256; i++) {
        mColorTable.append(qRgb(i,i,i));
    }
}

Palette::Palette(const QVector<char> &rgbs, bool sixBitsColor) {
    if (rgbs.size() != 768) {
        throw Status(-1, QStringLiteral("Palette description does not contain 256 colors"));
    }
    QDataStream imgDataStream(QByteArray((rgbs.constData()), rgbs.size()));
    quint8 r, g, b;
    for (int i=0; i<256; i++) {
        imgDataStream >> r >> g >> b;
        if (sixBitsColor) {
            r = r << 2u;
            g = g << 2u;
            b = b << 2u;
        }
        mColorTable.append(qRgb(r, g, b));
    }
}

QVector<QRgb> Palette::getColorTable() const
{
    return mColorTable;
}
