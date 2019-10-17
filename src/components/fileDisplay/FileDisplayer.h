#ifndef FILEDISPLAYER_H
#define FILEDISPLAYER_H

#include <QVBoxLayout>
#include "ImageDisplayer.h"
#include "bsa/BsaFile.h"

/**
 * @brief Class used to display the various file types found in a bsa archive.
 */
class FileDisplayer : public QVBoxLayout
{
    Q_OBJECT
signals:

public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Default constructor
     * @param parent of the FileDisplayer
     */
    explicit FileDisplayer(QWidget *parent = Q_NULLPTR);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************

public slots:
    void display(const BsaFile &file, const QVector<char> &imgData);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief Image displayer
     */
    ImageDisplayer *mImageDisplayer;
};

#endif // FILEDISPLAYER_H
