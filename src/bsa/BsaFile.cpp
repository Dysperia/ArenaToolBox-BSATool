#include "BsaFile.h"

//******************************************************************************
// Constructors
//******************************************************************************

BsaFile::BsaFile()
{

}

BsaFile::BsaFile(const quint32 &size,
                 const qint64 &startOffsetInArchive,
                 const QString fileName,
                 const quint16 &index):
    mSize(size), mStartOffsetInArchive(startOffsetInArchive),
    mFileName(fileName), mIndex(index)
{

}

//**************************************************************************
// Operators
//**************************************************************************

bool BsaFile::operator ==(const BsaFile &bsaFile) const
{
    if (mFileName != bsaFile.mFileName) {
        return false;
    }
    else if (mIndex != bsaFile.mIndex) {
        return false;
    }
    else {
        // TODO size comparison according to updated state
    }
}

bool BsaFile::operator !=(const BsaFile &bsaFile) const
{
    return !(this->operator==(bsaFile));
}

//******************************************************************************
// Getters/setters
//******************************************************************************
quint32 BsaFile::size() const
{
    return mSize;
}

qint64 BsaFile::startOffsetInArchive() const
{
    return mStartOffsetInArchive;
}

QString BsaFile::fileName() const
{
    return mFileName;
}

quint16 BsaFile::index() const
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

bool BsaFile::toDelete() const
{
    return mToDelete;
}

void BsaFile::setToDelete(bool toDelete)
{
    mToDelete = toDelete;
}

bool BsaFile::updated() const
{
    return mUpdated;
}

void BsaFile::setUpdated(bool updated)
{
    mUpdated = updated;
}

quint32 BsaFile::updateFileSize() const
{
    return mUpdateFileSize;
}

void BsaFile::setUpdateFileSize(quint32 updateFileSize)
{
    mUpdateFileSize = updateFileSize;
}

QString BsaFile::updateFilePath() const
{
    return mUpdateFilePath;
}

void BsaFile::setUpdateFilePath(const QString &updateFilePath)
{
    mUpdateFilePath = updateFilePath;
}

QString BsaFile::newFilePath() const
{
    return mNewFilePath;
}

void BsaFile::setNewFilePath(const QString &newFilePath)
{
    mNewFilePath = newFilePath;
}
