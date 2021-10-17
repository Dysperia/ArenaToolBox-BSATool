#ifndef IMAGEVIEWER_HPP
#define IMAGEVIEWER_HPP

#include <QWidget>
#include <QLabel>

class ImageViewer : public QWidget
{
    Q_OBJECT

private:

    // Set to true if the image has transparency
    bool hasTransparency;

    // Declaring some widgets and objects to be modifiable later
    QImage viewerQImage;
    QPixmap viewerPixmap;
    QLabel *viewerLabel;

    // Name of the file used to build the image
    std::string fileName;

    // Name of the palette used to create the last preview from building or filter change
    std::string PaletteName;

    // Integrated palette used to create the preview if any
    QVector<QRgb> integratedPalette;

    // Palette to store the actual used palette (other than Integrated)
    QVector<QRgb> actualPalette;

public:

    // contructor from file path
    ImageViewer(const QString &filePath, const QString &title, QWidget *parent = 0);

    // contructor from QImage
    ImageViewer(const QImage &image, const QString &filename, const QString &title, bool transparency, const std::string &defPalette = "PAL.COL",
                QVector<QRgb> integratedpalette = QVector<QRgb>(0), QWidget *parent = 0);

private slots:

    // update the viewer QLabel image when a new filter is selected
    void updatePreviewSlot(const QString &palette);
};

#endif // IMAGEVIEWER_HPP
