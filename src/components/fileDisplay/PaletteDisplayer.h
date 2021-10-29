#ifndef BSATOOL_PALETTEDISPLAYER_H
#define BSATOOL_PALETTEDISPLAYER_H

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <components/fileDisplay/ImageDisplayer.h>

/**
 * Display the colors of a palette
 */
class PaletteDisplayer : public QWidget {

//**************************************************************************
// Attributes
//**************************************************************************

public:
    //**************************************************************************
    // Constructor
    //**************************************************************************
    /**
     * Default constructor. Display the default "no preview" text as a single image collection
     * @param parent parent of this widget
     */
    explicit PaletteDisplayer(const QVector<QRgb> &palette, QWidget *parent = Q_NULLPTR);
};


#endif //BSATOOL_PALETTEDISPLAYER_H
