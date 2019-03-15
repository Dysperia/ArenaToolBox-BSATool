#include "FileDisplayer.h"
#include "../assets/Img.h"

//**************************************************************************
// Constructors
//**************************************************************************
FileDisplayer::FileDisplayer(QWidget *parent): QVBoxLayout (parent)
{
    this->mImageDisplayer = new ImageDisplayer;
    this->addWidget(this->mImageDisplayer);
}

void FileDisplayer::display(BsaFile &file, QVector<uchar> &imgData) {
    Img img(imgData);
    this->mImageDisplayer->display(img);
}
