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

void FileDisplayer::display(const BsaFile &file, const QVector<char> &fileData) {
    if (FileType::getExtension(file) == FileType::SET) {
        Img img(fileData, 64, static_cast<quint16>(file.size() / 64));
        this->mImageDisplayer->display(img);
    }
    else if (FileType::getExtension(file) == FileType::IMG) {
        Img img = file.size() == 4096 ? Img(fileData, 64, 64) : Img(fileData);
        this->mImageDisplayer->display(img);
    }
}
