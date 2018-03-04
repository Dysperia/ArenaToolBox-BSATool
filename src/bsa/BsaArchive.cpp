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
    return mArchiveFilePath;
}

uint16_t BsaArchive::getFileNumber() const
{
    return mFileNumber;
}

size_t BsaArchive::getBsaSize() const
{
    return mBsaSize;
}

int BsaArchive::getModifiedSize() const
{
    return mModifiedSize;
}

std::vector<BsaFile> BsaArchive::getFiles() const
{
    return mFiles;
}

bool BsaArchive::isOpened() const
{
    return mOpened;
}

bool BsaArchive::isModified() const
{
    return mModified;
}

//**************************************************************************
// Methods
//**************************************************************************
Error BsaArchive::openArchive(const std::string &filePath)
{
    //TODO implements openArchive
    return Error(1);
}

void BsaArchive::clear()
{
    //TODO implements clear
}

Error BsaArchive::extractFile(const std::string &destinationFolder, const BsaFile &file)
{
    //TODO implements extractFile
    return Error(1);
}

BsaFile BsaArchive::updateFile(const std::string &updateFilePath, const BsaFile &file)
{
    //TODO implements updateFile
    return BsaFile(0,0,"",0);
}

BsaFile BsaArchive::deleteFile(const BsaFile &file)
{
    //TODO implements deleteFile
    return BsaFile(0,0,"",0);
}

BsaFile BsaArchive::addFile(const std::string &filePath)
{
    //TODO implements addFile
    return BsaFile(0,0,"",0);
}

BsaFile BsaArchive::cancelDeleteFile(const BsaFile &file)
{
    //TODO implements cancelDeleteFile
    return BsaFile(0,0,"",0);
}

BsaFile BsaArchive::cancelUpdateFile(const BsaFile &file)
{
    //TODO implements cancelUpdateFile
    return BsaFile(0,0,"",0);
}

void BsaArchive::createNewArchive()
{
    //TODO implements createNewArchive
}

Error BsaArchive::saveArchive(const std::string &filePath)
{
    //TODO implements saveArchive
    return Error(1);
}
