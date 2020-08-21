#ifndef BSATOOL_IMAGEDISPLAYER_H
#define BSATOOL_IMAGEDISPLAYER_H

#include <QWheelEvent>
#include <QScrollArea>
#include <QtWidgets/QLabel>
#include "assets/Img.h"

/**
 * Single image displayer. It allows zooming and dragging of the image. The image is first displayed at native size if
 * it fits in the displayer, displayed reduced if not in order to see the whole image
 */
class ImageDisplayer : public QScrollArea {
public:
    //**************************************************************************
    // Statics
    //**************************************************************************
    /**
     * Factor for zoom in
     */
    constexpr static double ZOOM_IN_FACTOR = 1.25;
    /**
     * Factor for zoom out
     */
    constexpr static double ZOOM_OUT_FACTOR = 0.8;

    //**************************************************************************
    // Constructor
    //**************************************************************************
    /**
     * Default constructor. Display the default "no preview" text
     * @param parent parent of this widget
     */
    explicit ImageDisplayer(QWidget *parent = Q_NULLPTR);

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Display an img or the default text if the img cannot be displayed
     * @param img to display
     */
    void display(const Img &img);

    /**
     * Display the default "no preview" text
     */
    void displayDefaultText();

    /**
     * Mouse wheel event listener used to zoom. Zooming is disabled if the user is moving the image
     * @param event from system
     */
    void wheelEvent(QWheelEvent *event) override;

    /**
     * Mouse move event used to move the image. Require pressing the left mouse button
     * @param event from system
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * Mouse move event used to start image moving. Require pressing the left mouse button
     * @param event from system
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * Mouse move event used to end image moving. Require releasing the left mouse button
     * @param event from system
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    //**************************************************************************
    // Statics
    //**************************************************************************
    /**
     * Adjust the scroll bars values to keep the image actual center when zooming
     * @param scrollBar to adjust
     * @param factor used when zooming
     * @param imageLength zoomed new image length in the scrollBar's direction
     * @param updateScrollBarMaximum true if the scrollBar maximum value must be updated in advance
     */
    static void adjustScrollBar(QScrollBar *scrollBar, double factor, int imageLength, bool updateScrollBarMaximum);

    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * Image to display
     */
    QImage mQImage;
    /**
     * Image or text displayer
     */
    QLabel mLabel;
    /**
     * Ratio used to resize image at current zoom setting
     */
    double mRatio = 1.0;
    /**
     * Image to display and the object resized when zooming
     */
    QPixmap mPixmap;
    /**
     * Horizontal scroll bar value when dragging started
     */
    int mHScrollBarOrigin = 0;
    /**
     * Vertical scroll bar value when dragging started
     */
    int mVScrollBarOrigin = 0;
    /**
     * Dragging occured if true (equals to left mouse button pressed)
     */
    bool mDragging = false;
    /**
     * Global mouse position when dragging started
     */
    QPoint mMouseOrigin;

    //**************************************************************************
    // Methods
    //**************************************************************************

    /**
     * Return whether an image can be displayed
     * @return true if the QImage and the QPixmap are valid
     */
    bool hasValidImage();

    /**
     * Zoom the displayed image
     * @param factor of zoom
     */
    void zoom(double factor);
};

#endif // BSATOOL_IMAGEDISPLAYER_H
