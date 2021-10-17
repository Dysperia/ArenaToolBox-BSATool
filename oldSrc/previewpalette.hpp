#ifndef PREVIEWPALETTE_HPP
#define PREVIEWPALETTE_HPP

#include <QWidget>

class PreviewPalette : public QWidget
{
public:
    PreviewPalette(const QVector<QRgb> palette, QWidget *parent = 0);
};

#endif // PREVIEWPALETTE_HPP
