#ifndef BSATOOL_IMAGESCOLLECTIONDISPLAYER_H
#define BSATOOL_IMAGESCOLLECTIONDISPLAYER_H

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include "ImageDisplayer.h"

/**
 * Display an image collection and allow the user to navigate the collection
 */
class ImagesCollectionDisplayer : public QWidget {
Q_OBJECT

//**************************************************************************
// Attributes
//**************************************************************************
private slots:
    /**
     * Display first image
     */
    void goFirst();
    /**
     * Display previous image
     */
    void goPrevious();
    /**
     * Display next image
     */
    void goNext();
    /**
     * Display last image
     */
    void goLast();
public:
    //**************************************************************************
    // Constructor
    //**************************************************************************
    /**
     * Default constructor. Display the default "no preview" text as a single image collection
     * @param parent parent of this widget
     */
    explicit ImagesCollectionDisplayer(QWidget *parent = Q_NULLPTR);

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Display a img collection or the default text if the collection cannot be displayed. It starts at the first image
     * @param imageCollection to display
     */
    virtual void display(const QVector<QImage> &imageCollection);

    /**
     * Update the disabled state of the buttons and the current index label
     */
    void updateButtonBar();

protected:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * List of images to display
     */
    QVector<QImage> mImageList;

    /**
     * List of "going left" buttons
     */
    QVector<QPushButton *> mLeftButtons;

    /**
     * List of "going right" buttons
     */
    QVector<QPushButton *> mRightButtons;

    /**
     * Index of the image to display
     */
    int mCurrentIndex = 0;

    /**
     * Display index of the image to display and the total number of images
     */
    QLabel mIndexDisplayer;

    /**
     * Single image displayer
     */
    ImageDisplayer mImageDisplayer;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Create a button with the given text and a minimal width
     * @param text to display on the button
     * @return the created button
     */
    [[nodiscard]] QPushButton *createButton(const QString& text) const;

    /**
     * display the image corresponding to the current index if it exists and update the buttons state
     */
    void displayCurrentSelectedImageAndUpdateButtons(bool resetZoom = false);
};


#endif //BSATOOL_IMAGESCOLLECTIONDISPLAYER_H
