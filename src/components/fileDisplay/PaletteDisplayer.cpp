#include <components/fileDisplay/PaletteDisplayer.h>
#include <QLabel>
#include <QGridLayout>
#include <QtMath>

//**************************************************************************
// Constructor
//**************************************************************************
PaletteDisplayer::PaletteDisplayer(const QVector<QRgb> &palette, QWidget *parent) : QWidget(parent) {
    auto *previewPaletteLayout = new QGridLayout;
    setLayout(previewPaletteLayout);
    QVector<QLabel*> indexLabels(palette.size());
    QVector<QLabel*> colorLabels(palette.size());
    QVector<QPixmap*> colorPixmaps(palette.size());
    int columnNb = qCeil(qSqrt(palette.size()));
    for (int i(0); i<palette.size(); i++)
    {
        colorPixmaps[i] = new QPixmap(16, 16);
        const QColor &color = QColor(palette[i]);
        colorPixmaps[i]->fill(color);
        colorLabels[i] = new QLabel;
        colorLabels[i]->resize(16, 16);
        colorLabels[i]->setPixmap(*colorPixmaps[i]);
        colorLabels[i]->setToolTip(color.name());
        indexLabels[i] = new QLabel;
        indexLabels[i]->resize(16, 16);
        indexLabels[i]->setTextFormat(Qt::PlainText);
        indexLabels[i]->setTextInteractionFlags(Qt::NoTextInteraction);
        indexLabels[i]->setText(QString::number(i));
        previewPaletteLayout->addWidget(colorLabels[i], i/columnNb*2, i%columnNb);
        previewPaletteLayout->addWidget(indexLabels[i], i/columnNb*2+1, i%columnNb);
    }
}
