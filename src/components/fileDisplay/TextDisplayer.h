#ifndef BSATOOL_TEXTDISPLAYER_H
#define BSATOOL_TEXTDISPLAYER_H

#include <QScrollArea>
#include <QtWidgets/QLabel>

/**
 * Displayer allowing to display a large text thanks to the scroll area
 */
class TextDisplayer : public QScrollArea {
public:
    //**************************************************************************
    // Constructor
    //**************************************************************************
    /**
     * Default constructor. Display the default "no preview" text
     * @param parent parent of this widget
     */
    explicit TextDisplayer(QWidget *parent = Q_NULLPTR);

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Display the given text
     * @param text to display
     */
    void display(QString &text);

private:

    /**
     * text displayer
     */
    QLabel mLabel;
};


#endif //BSATOOL_TEXTDISPLAYER_H
