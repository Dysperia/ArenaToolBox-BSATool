#include "ImageDisplayer.h"
#include <QScrollBar>

//**************************************************************************
// Constructor
//**************************************************************************
ImageDisplayer::ImageDisplayer(QWidget *parent) : QScrollArea(parent) {
    this->mLabel.setScaledContents(true);
    this->mLabel.setAlignment(Qt::AlignCenter | Qt::AlignHCenter);

    this->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    this->setMinimumSize(322, 202);
    this->setWidget(&this->mLabel);

    displayDefaultText();
}

//**************************************************************************
// Methods
//**************************************************************************
void ImageDisplayer::displayDefaultText() {
    mLabel.setText(QString("No preview available for this file."));
    mLabel.resize(this->size().width() - 2, this->size().height() - 2);
}

void ImageDisplayer::display(Img &img) {
    mRatio = 1.0;
    this->mQImage = img.qImage();
    if (this->mQImage.isNull()) {
        displayDefaultText();
    } else {
        mPixmap = QPixmap();
        if (mPixmap.convertFromImage(this->mQImage)) {
            this->mLabel.setPixmap(mPixmap);
            this->mLabel.resize(mPixmap.size());
            double ratioWidth = 1.0 * mPixmap.size().width() / (this->size().width() - 2);
            double ratioHeight = 1.0 * mPixmap.size().height() / (this->size().height() - 2);
            if (ratioHeight > 1.0 || ratioWidth > 1.0) {
                zoom(1.0 / std::max(ratioWidth, ratioHeight));
            }
        } else {
            displayDefaultText();
        }
    }
}

void ImageDisplayer::adjustScrollBar(QScrollBar *scrollBar, double factor, int imageLength, bool updateScrollBarMaximum)
{
    if (updateScrollBarMaximum) {
        scrollBar->setMaximum(imageLength - scrollBar->pageStep());
    }
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageDisplayer::wheelEvent(QWheelEvent *event) {
    if (hasValidImage() && !mDragging) {
        QPoint numDegrees = event->angleDelta() / 8;
        if (!numDegrees.isNull()) {
            int numSteps = numDegrees.y() / 15;
            if (numSteps > 0 && mRatio * ZOOM_IN_FACTOR <= 20) {
                zoom(ZOOM_IN_FACTOR);
            } else if (numSteps < 0 && mRatio * ZOOM_OUT_FACTOR >= 0.2) {
                zoom(ZOOM_OUT_FACTOR);
            }
        }
    }
    event->accept();
}

void ImageDisplayer::zoom(double factor) {
    bool isZoomIn = factor == ZOOM_IN_FACTOR;
    mRatio *= factor;
    QSize newImageSize = mPixmap.size() * mRatio;
    adjustScrollBar(horizontalScrollBar(), factor, newImageSize.width(), isZoomIn && newImageSize.width() > this->width());
    adjustScrollBar(verticalScrollBar(), factor, newImageSize.height(), isZoomIn && newImageSize.height() > this->height());
    mLabel.setPixmap(mPixmap.scaled(mPixmap.size() * mRatio, Qt::KeepAspectRatio, Qt::FastTransformation));
    mLabel.resize(mLabel.pixmap()->size());
}

void ImageDisplayer::mouseMoveEvent(QMouseEvent *event) {
    if (hasValidImage() && mDragging) {
        int deltaX = event->globalPos().x() - mMouseOrigin.x();
        int deltaY = event->globalPos().y() - mMouseOrigin.y();
        horizontalScrollBar()->setValue(mHScrollBarOrigin - deltaX);
        verticalScrollBar()->setValue(mVScrollBarOrigin - deltaY);
    }
    event->accept();
}

void ImageDisplayer::mousePressEvent(QMouseEvent *event) {
    if (hasValidImage() && event->button() == Qt::LeftButton) {
        mDragging = true;
        mMouseOrigin = event->globalPos();
        mHScrollBarOrigin = horizontalScrollBar()->value();
        mVScrollBarOrigin = verticalScrollBar()->value();
    }
    event->accept();
}

void ImageDisplayer::mouseReleaseEvent(QMouseEvent *event) {
    if (hasValidImage() && event->button() == Qt::LeftButton) {
        mDragging = false;
    }
    event->accept();
}

bool ImageDisplayer::hasValidImage() {
    return !mQImage.isNull() && !mPixmap.isNull();
}
