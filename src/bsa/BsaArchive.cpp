#include "BsaArchive.h"

//******************************************************************************
// Constructors
//******************************************************************************
BsaArchive::BsaArchive()
{

}

//******************************************************************************
// Getters/setters
//******************************************************************************
std::string BsaArchive::getArchiveFilePath() const
{
    return archiveFilePath;
}

uint16_t BsaArchive::getFileNumber() const
{
    return fileNumber;
}

size_t BsaArchive::getBsaSize() const
{
    return bsaSize;
}

int BsaArchive::getModifiedSize() const
{
    return modifiedSize;
}

std::vector<BsaFile> BsaArchive::getFiles() const
{
    return files;
}

bool BsaArchive::isOpened() const
{
    return opened;
}

bool BsaArchive::isModified() const
{
    return modified;
}
