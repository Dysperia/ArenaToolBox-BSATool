#include "imageviewer.hpp"
#include "globalImage.hpp"
#include <QVBoxLayout>
#include <QComboBox>

// contructor from file path
ImageViewer::ImageViewer(const QString &filePath, const QString &title, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *viewerLayout = new QVBoxLayout;
    setLayout(viewerLayout);
    QHBoxLayout *viewerFilterLayout = new QHBoxLayout;
    QLabel *viewerFilterLabel = new QLabel("Palette");
    QComboBox *viewerPaletteFilter = new QComboBox;
    QStringList paletteList = image.getPaletteFileList();
    viewerPaletteFilter->addItems(paletteList);
    connect(viewerPaletteFilter, SIGNAL(activated(QString)), this, SLOT(updatePreviewSlot(QString)));
    viewerFilterLayout->addWidget(viewerFilterLabel);
    viewerFilterLayout->addWidget(viewerPaletteFilter);
    viewerLayout->addLayout(viewerFilterLayout);
    viewerLabel = new QLabel("No preview for this file.\n\n"
                                                "Either due to an error while opening image\n"
                                                "or IMG/SET has a bad format.");
    viewerLabel->setAlignment(Qt::AlignCenter);
    viewerLabel->setFixedSize(320, 200);
    viewerLayout->addWidget(viewerLabel);
    if (filePath != NULL)
    {
        viewerQImage = image.buildQImage(filePath.toStdString());
        if (viewerQImage.size() != QSize(0,0))
        {
            std::string cFilePath = filePath.toStdString();
            int fileNamePos = cFilePath.find_last_of("/\\");
            fileName = cFilePath.substr(fileNamePos + 1);
            setWindowTitle(title + " : " + QString::fromStdString(fileName));
            PaletteName = image.getPaletteName();
            if (PaletteName == "Integrated")
            {
                // special when file is native and should have palette integrated but this file has not (header flag set to none integrated)
                if ((image.getImageFlags() & 0xFF00) == 0x0100 || (image.getImageFlags() & 0xFF00) == 0x0900)
                {
                    viewerPaletteFilter->addItem("Integrated");
                    hasTransparency = image.getTransparency();
                    integratedPalette = image.getPalette(PaletteName, hasTransparency);
                }
                else
                {
                    PaletteName = "PAL.COL";
                }
            }
            viewerPaletteFilter->setCurrentText(QString::fromStdString(PaletteName));
            updatePreviewSlot(QString::fromStdString(PaletteName));
        }
    }
}

// contructor from QImage
ImageViewer::ImageViewer(const QImage &qimage, const QString &filename, const QString &title, bool transparency, const std::string &defPalette,
                         QVector<QRgb> integratedpalette, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *viewerLayout = new QVBoxLayout;
    setLayout(viewerLayout);
    QHBoxLayout *viewerFilterLayout = new QHBoxLayout;
    QLabel *viewerFilterLabel = new QLabel("Palette");
    QComboBox *viewerPaletteFilter = new QComboBox;
    QStringList paletteList = image.getPaletteFileList();
    viewerPaletteFilter->addItems(paletteList);
    connect(viewerPaletteFilter, SIGNAL(activated(QString)), this, SLOT(updatePreviewSlot(QString)));
    viewerFilterLayout->addWidget(viewerFilterLabel);
    viewerFilterLayout->addWidget(viewerPaletteFilter);
    viewerLayout->addLayout(viewerFilterLayout);
    viewerLabel = new QLabel;
    viewerLabel->setAlignment(Qt::AlignCenter);
    viewerLabel->setFixedSize(320, 200);
    viewerLayout->addWidget(viewerLabel);
    viewerQImage = qimage;
    hasTransparency = transparency;
    if (viewerQImage.size() != QSize(0,0))
    {
        fileName = filename.toStdString();
        setWindowTitle(title + " : " + filename);
        PaletteName = defPalette;
        integratedPalette = integratedpalette;
        if (integratedPalette.size() != 0)
        {
            viewerPaletteFilter->addItem("Integrated");
        }
        viewerPaletteFilter->setCurrentText(QString::fromStdString(PaletteName));
        updatePreviewSlot(QString::fromStdString(PaletteName));
    }
}

// update the viewer Qlabel with the image build with the given palette
void ImageViewer::updatePreviewSlot(const QString &palette)
{
    if (palette == "Integrated")
    {
        PaletteName = "Integrated";
        viewerQImage.setColorTable(integratedPalette);
    }
    else
    {
        PaletteName = palette.toStdString();
        actualPalette = image.getPalette(PaletteName, hasTransparency);
        viewerQImage.setColorTable(actualPalette);
    }
    viewerPixmap.convertFromImage(viewerQImage);
    if (viewerPixmap.height() > 200 || viewerPixmap.width() > 320)
    {
        if ((viewerPixmap.height() - 200) < (viewerPixmap.width() - 320))
        {
            viewerPixmap = viewerPixmap.scaledToWidth(320);
        }
        else
        {
            viewerPixmap = viewerPixmap.scaledToHeight(200);
        }
    }
    viewerLabel->setPixmap(viewerPixmap);
}
