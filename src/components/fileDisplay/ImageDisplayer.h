#ifndef BSATOOL_IMAGEDISPLAYER_H
#define BSATOOL_IMAGEDISPLAYER_H

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

#endif // BSATOOL_IMAGEDISPLAYER_H
