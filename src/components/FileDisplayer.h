#ifndef FILEDISPLAYER_H
#define FILEDISPLAYER_H

#include <QVBoxLayout>
#include "ImageDisplayer.h"

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
    FileDisplayer(QWidget *parent = Q_NULLPTR);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************

public slots:

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
};

#endif // FILEDISPLAYER_H
