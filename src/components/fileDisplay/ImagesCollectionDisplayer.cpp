#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <components/fileDisplay/ImagesCollectionDisplayer.h>

//**************************************************************************
// Constructor
//**************************************************************************
ImagesCollectionDisplayer::ImagesCollectionDisplayer(QWidget *parent) : QWidget(parent) {
    auto buttonsLayout = new QHBoxLayout();
    auto firstButton = createButton(QString("<<"));
    connect(firstButton, &QAbstractButton::clicked, this, &ImagesCollectionDisplayer::goFirst);
    mLeftButtons.push_back(firstButton);
    auto previousButton = createButton(QString("<"));
    connect(previousButton, &QAbstractButton::clicked, this, &ImagesCollectionDisplayer::goPrevious);
    mLeftButtons.push_back(previousButton);
    auto nextButton = createButton(QString(">"));
    connect(nextButton, &QAbstractButton::clicked, this, &ImagesCollectionDisplayer::goNext);
    mRightButtons.push_back(nextButton);
    auto lastButton = createButton(QString(">>"));
    connect(lastButton, &QAbstractButton::clicked, this, &ImagesCollectionDisplayer::goLast);
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

QPushButton *ImagesCollectionDisplayer::createButton(const QString& text) {
    auto button = new QPushButton(text);
    button->setDisabled(true);
    button->setMinimumWidth(10);
    return button;
}

//**************************************************************************
// Methods
//**************************************************************************
void ImagesCollectionDisplayer::display(const QVector<QImage> &imageCollection) {
    mImageList.clear();
    for (const auto &image : imageCollection) {
        mImageList.push_back(image.copy());
    }
    if (mImageList.empty()) {
        mImageDisplayer.displayDefaultText();
        updateButtonBar();
    }
    else {
        mCurrentIndex = 0;
        displayCurrentSelectedImageAndUpdateButtons(true);
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
    displayCurrentSelectedImageAndUpdateButtons();
}

void ImagesCollectionDisplayer::goPrevious() {
    if (mCurrentIndex > 0) {
        mCurrentIndex--;
    }
    displayCurrentSelectedImageAndUpdateButtons();
}

void ImagesCollectionDisplayer::goNext() {
    if (mCurrentIndex < mImageList.size() - 1) {
        mCurrentIndex++;
    }
    displayCurrentSelectedImageAndUpdateButtons();
}

void ImagesCollectionDisplayer::goLast() {
    mCurrentIndex = mImageList.size() - 1;
    displayCurrentSelectedImageAndUpdateButtons();
}

void ImagesCollectionDisplayer::displayCurrentSelectedImageAndUpdateButtons(bool resetZoom) {
    mImageDisplayer.display(mImageList.at(mCurrentIndex), resetZoom);
    updateButtonBar();
}
