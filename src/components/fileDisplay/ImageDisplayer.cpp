#include "ImageDisplayer.h"

ImageDisplayer::ImageDisplayer()
{
    this->setAlignment(Qt::AlignCenter);
    this->setFixedSize(320, 200);
    this->setText(QString("No preview available for this file."));
}

void ImageDisplayer::display(Img &img)
{
    this->mQImage = img.qImage();
    if (this->mQImage.isNull()) {
        this->setText(QString("No preview available for this file."));
    }
    else {
        QPixmap qPixmap;
        qPixmap.convertFromImage(this->mQImage);
        if (qPixmap.height() > 200 || qPixmap.width() > 320)
            {
                if ((qPixmap.height() - 200) < (qPixmap.width() - 320))
                {
                    qPixmap = qPixmap.scaledToWidth(320);
                }
                else
                {
                    qPixmap = qPixmap.scaledToHeight(200);
                }
            }
        this->setPixmap(qPixmap);
    }
}
