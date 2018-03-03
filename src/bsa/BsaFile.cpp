#include "BsaFile.h"

//******************************************************************************
// Constructors
//******************************************************************************
BsaFile::BsaFile(const uint32_t &size,
           const size_t &startOffsetInArchive,
           const std::string fileName) :
    mSize(size),
    mStartOffsetInArchive(startOffsetInArchive),
    mFileName(fileName)
{

}

//******************************************************************************
// Getters/setters
//******************************************************************************
uint32_t BsaFile::size() const
{
    return mSize;
}

size_t BsaFile::startOffsetInArchive() const
{
    return mStartOffsetInArchive;
}

const std::string BsaFile::fileName() const
{
    return mFileName;
}

bool BsaFile::isNew() const
{
    return mIsNew;
}

void BsaFile::setIsNew(bool isNew)
{
    mIsNew = isNew;
}

bool BsaFile::toDeleted() const
{
    return mToDeleted;
}

void BsaFile::setToDeleted(bool toDeleted)
{
    mToDeleted = toDeleted;
}

bool BsaFile::updated() const
{
    return mUpdated;
}

void BsaFile::setUpdated(bool updated)
{
    mUpdated = updated;
}

int BsaFile::updateFileSize() const
{
    return mUpdateFileSize;
}

void BsaFile::setUpdateFileSize(int updateFileSize)
{
    mUpdateFileSize = updateFileSize;
}

std::string BsaFile::updateFilePath() const
{
    return mUpdateFilePath;
}

void BsaFile::setUpdateFilePath(const std::string &updateFilePath)
{
    mUpdateFilePath = updateFilePath;
}

std::string BsaFile::newFilePath() const
{
    return mNewFilePath;
}

void BsaFile::setNewFilePath(const std::string &newFilePath)
{
    mNewFilePath = newFilePath;
}
