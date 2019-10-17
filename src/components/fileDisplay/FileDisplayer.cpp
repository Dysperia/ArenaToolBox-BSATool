#include <assets/FileType.h>
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
    if (file.size() == 4096) {
        Img img(imgData, 64, 64);
        this->mImageDisplayer->display(img);
    }
    else if (FileType::getExtension(file) == FileType::SET) {
        Img img(imgData, 64, static_cast<quint16>(file.size() / 64));
        this->mImageDisplayer->display(img);
    }
    else {
        Img img(imgData);
        this->mImageDisplayer->display(img);
    }
}
