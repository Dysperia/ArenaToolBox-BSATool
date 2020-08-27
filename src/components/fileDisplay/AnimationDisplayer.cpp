#include "AnimationDisplayer.h"
#include <QVBoxLayout>
#include <QDebug>

//**************************************************************************
// Constructor
//**************************************************************************
AnimationDisplayer::AnimationDisplayer(QWidget *parent) : ImagesCollectionDisplayer(parent) {
    auto *layout = (QVBoxLayout *)this->layout();
    mPlayAnimationButton = new QPushButton("Start animation");
    connect(mPlayAnimationButton, SIGNAL(clicked(bool)), SLOT(toggleAnimation()));
    layout->addWidget(mPlayAnimationButton);
    mTimer.setInterval(1000 / 12);
    connect(&mTimer, SIGNAL(timeout()), SLOT(goNextForAnimation()));
}

//**************************************************************************
// Methods
//**************************************************************************
void AnimationDisplayer::display(const QVector<QImage> &imageCollection) {
    ImagesCollectionDisplayer::display(imageCollection);
}

void AnimationDisplayer::goNextForAnimation() {
    if (!mImageList.empty()) {
        mCurrentIndex = (mCurrentIndex + 1) % mImageList.size();
        mImageDisplayer.display(mImageList.at(mCurrentIndex), false);
        mIndexDisplayer.setText(mImageList.empty() ? "/" : (QString::number(mCurrentIndex + 1) + "/" + QString::number(mImageList.size())));
    }
}

void AnimationDisplayer::toggleAnimation() {
    if (mTimer.isActive()) {
        mTimer.stop();
        mPlayAnimationButton->setText(QStringLiteral("Start animation"));
        updateButtonBar();
    }
    else {
        for (auto b : mLeftButtons) {
            b->setDisabled(true);
        }
        for (auto b : mRightButtons) {
            b->setDisabled(true);
        }
        mPlayAnimationButton->setText(QStringLiteral("Stop animation"));
        mTimer.start();
    }
}

void AnimationDisplayer::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    if (mStoppedByHideEvent) {
        toggleAnimation();
    }
}

void AnimationDisplayer::hideEvent(QHideEvent *event) {
    if (mTimer.isActive()) {
        mStoppedByHideEvent = true;
        toggleAnimation();
    }
    QWidget::hideEvent(event);
}
