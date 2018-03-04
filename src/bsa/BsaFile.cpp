#include "BsaFile.h"

//******************************************************************************
// Constructors
//******************************************************************************
BsaFile::BsaFile(const uint32_t &size,
           const size_t &startOffsetInArchive,
           const std::string fileName,
           const size_t &index) :
    mSize(size), mStartOffsetInArchive(startOffsetInArchive),
    mFileName(fileName), mIndex(index)
{

}

BsaFile::BsaFile(const BsaFile &bsaFile)
{
    BsaFile(bsaFile.mSize, bsaFile.mStartOffsetInArchive, bsaFile.mFileName,
            bsaFile.mIndex);
    mIsNew = bsaFile.mIsNew;
    mToDeleted = bsaFile.mToDeleted;
    mUpdated = bsaFile.mUpdated;
    mUpdateFileSize = bsaFile.mUpdateFileSize;
    mUpdateFilePath = bsaFile.mUpdateFilePath;
    mNewFilePath = bsaFile.mNewFilePath;
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

std::string BsaFile::fileName() const
{
    return mFileName;
}

size_t BsaFile::index() const
{
    return mIndex;
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
