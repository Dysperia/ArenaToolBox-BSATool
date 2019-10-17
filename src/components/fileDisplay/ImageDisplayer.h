#ifndef IMAGEDISPLAYER_H
#define IMAGEDISPLAYER_H

#include <QLabel>
#include "assets/Img.h"

class ImageDisplayer : public QLabel
{
public:
    ImageDisplayer();
    void display(Img &img);

private:
    QImage mQImage;
};

#endif // IMAGEDISPLAYER_H
