#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include "ImagesCollectionDisplayer.h"

//**************************************************************************
// Constructor
//**************************************************************************
ImagesCollectionDisplayer::ImagesCollectionDisplayer(QWidget *parent) : QWidget(parent) {
    auto buttonsLayout = new QHBoxLayout();
    auto firstButton = createButton(QString("<<"));
    connect(firstButton, SIGNAL(clicked(bool)), this, SLOT(goFirst()));
    mLeftButtons.push_back(firstButton);
    auto previousButton = createButton(QString("<"));
    connect(previousButton, SIGNAL(clicked(bool)), this, SLOT(goPrevious()));
    mLeftButtons.push_back(previousButton);
    auto nextButton = createButton(QString(">"));
    connect(nextButton, SIGNAL(clicked(bool)), this, SLOT(goNext()));
    mRightButtons.push_back(nextButton);
    auto lastButton = createButton(QString(">>"));
    connect(lastButton, SIGNAL(clicked(bool)), this, SLOT(goLast()));
    mRightButtons.push_back(lastButton);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(firstButton);
    buttonsLayout->addWidget(previousButton);
    buttonsLayout->addWidget(&mIndexDisplayer);
    buttonsLayout->addWidget(nextButton);
    buttonsLayout->addWidget(lastButton);
    buttonsLayout->addStretch(1);

    auto mainVBox = new QVBoxLayout();
    mainVBox->addWidget(&mImageDisplayer, 1);
    mainVBox->addLayout(buttonsLayout);
    mainVBox->setAlignment(Qt::AlignCenter);

    this->setLayout(mainVBox);

    updateButtonBar();
}

QPushButton *ImagesCollectionDisplayer::createButton(const QString& text) const {
    auto button = new QPushButton(text);
    button->setDisabled(true);
    button->setMinimumWidth(10);
    return button;
}

//**************************************************************************
// Methods
//**************************************************************************
void ImagesCollectionDisplayer::display(const QVector<Img> &imgCollection) {
    mImageList = imgCollection;
    if (imgCollection.empty()) {
        mImageDisplayer.displayDefaultText();
        updateButtonBar();
    }
    else {
        goFirst();
    }
}

void ImagesCollectionDisplayer::updateButtonBar() {
    for (auto b : mLeftButtons) {
        b->setDisabled(mImageList.empty() || mCurrentIndex == 0);
    }
    for (auto b : mRightButtons) {
        b->setDisabled(mImageList.empty() || mCurrentIndex == mImageList.size() - 1);
    }
    mIndexDisplayer.setText(mImageList.empty() ? "/" : (QString::number(mCurrentIndex + 1) + "/" + QString::number(mImageList.size())));
}

void ImagesCollectionDisplayer::goFirst() {
    mCurrentIndex = 0;
    mImageDisplayer.display(mImageList.at(mCurrentIndex));
    updateButtonBar();
}

void ImagesCollectionDisplayer::goPrevious() {
    mCurrentIndex--;
    mImageDisplayer.display(mImageList.at(mCurrentIndex));
    updateButtonBar();
}

void ImagesCollectionDisplayer::goNext() {
    mCurrentIndex++;
    mImageDisplayer.display(mImageList.at(mCurrentIndex));
    updateButtonBar();
}

void ImagesCollectionDisplayer::goLast() {
    mCurrentIndex = mImageList.size() - 1;
    mImageDisplayer.display(mImageList.at(mCurrentIndex));
    updateButtonBar();
}
