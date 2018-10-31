#ifndef IMAGEDISPLAYER_H
#define IMAGEDISPLAYER_H

#include <QLabel>

class ImageDisplayer : public QLabel
{
public:
    ImageDisplayer();
    void display(QImage &img);
};

#endif // IMAGEDISPLAYER_H
