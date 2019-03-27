#include "FileDisplayer.h"

//**************************************************************************
// Constructors
//**************************************************************************
FileDisplayer::FileDisplayer(QWidget *parent): QVBoxLayout (parent)
{
    this->mImageDisplayer = new ImageDisplayer;
    this->addWidget(this->mImageDisplayer);
}

void FileDisplayer::display(const BsaFile &file, const QVector<char> &imgData) {
    Img img(imgData);
    this->mImageDisplayer->display(img);
}
