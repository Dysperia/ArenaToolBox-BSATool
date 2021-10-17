#ifndef BSATOOL_ANIMATIONDISPLAYER_H
#define BSATOOL_ANIMATIONDISPLAYER_H

#include <QObject>
#include <QTimer>
#include <components/fileDisplay/ImageDisplayer.h>
#include <components/fileDisplay/ImagesCollectionDisplayer.h>

/**
 * Displayer able to show an running animation
 */
class AnimationDisplayer : public ImagesCollectionDisplayer {
Q_OBJECT

private slots:
    //**************************************************************************
    // Slots
    //**************************************************************************
    /**
     * Display next animation image
     */
    void goNextForAnimation();
    /**
     * Start / stop playing the animation
     */
    void toggleAnimation();

public:
    //**************************************************************************
    // Constructor
    //**************************************************************************
    /**
     * Default constructor. Display the default "no preview" text as a single image animation
     * @param parent parent of this widget
     */
    explicit AnimationDisplayer(QWidget *parent = Q_NULLPTR);

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Display an animation or the default text if the animation cannot be displayed. It starts at the first image
     * @param imageCollection to display
     */
    void display(const QVector<QImage> &imageCollection) override;
    /**
     * Show event management to restart the animation if running before hide event received
     * @param event show event
     */
    void showEvent(QShowEvent *event) override;
    /**
     * Hide event management to stop the animation if running when hide event received
     * @param event hide event
     */
    void hideEvent(QHideEvent *event) override;

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * Timer of the animation
     */
    QTimer mTimer;
    /**
     * Tell whether the timer has been stopped by the user or because of a hide event
     */
    bool mStoppedByHideEvent = false;
    /**
     * Button used to play / stop the animation
     */
    QPushButton *mPlayAnimationButton;
};


#endif //BSATOOL_ANIMATIONDISPLAYER_H
