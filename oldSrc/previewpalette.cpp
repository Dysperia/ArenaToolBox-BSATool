#include "previewpalette.hpp"
#include <QLabel>
#include <QGridLayout>

// Palette is a 256 color palette
PreviewPalette::PreviewPalette(const QVector<QRgb> palette, QWidget *parent) : QWidget(parent)
{
    QGridLayout *previewPaletteLayout = new QGridLayout;
    setLayout(previewPaletteLayout);
    QVector<QLabel*> indexLabels(256);
    QVector<QLabel*> colorLabels(256);
    QVector<QPixmap*> colorPixmaps(256);
    for (int i(0); i<256; i++)
    {
        colorPixmaps[i] = new QPixmap(16, 16);
        colorPixmaps[i]->fill(QColor(palette[i]));
        colorLabels[i] = new QLabel;
        colorLabels[i]->resize(16, 16);
        colorLabels[i]->setPixmap(*colorPixmaps[i]);
        indexLabels[i] = new QLabel;
        indexLabels[i]->resize(16, 16);
        indexLabels[i]->setTextFormat(Qt::PlainText);
        indexLabels[i]->setTextInteractionFlags(Qt::NoTextInteraction);
        indexLabels[i]->setText(QString::number(i, 16).toUpper());
        previewPaletteLayout->addWidget(colorLabels[i], i/16*2, i%16);
        previewPaletteLayout->addWidget(indexLabels[i], i/16*2+1, i%16);
    }
}
