#include "FileListViewerItem.h"

//******************************************************************************
// Constructors
//******************************************************************************
FileListViewerItem::FileListViewerItem(QListWidget *parent): QListWidgetItem(parent)
{

}

//******************************************************************************
// Getters/setters
//******************************************************************************
BsaFile FileListViewerItem::bsaFile() const
{
    return mBsaFile;
}

void FileListViewerItem::setBsaFile(const BsaFile &bsaFile)
{
    mBsaFile = bsaFile;
}
