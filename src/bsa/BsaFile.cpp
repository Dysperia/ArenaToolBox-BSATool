#include <utility>

#include "BsaFile.h"

//******************************************************************************
// Statics
//******************************************************************************
const BsaFile BsaFile::INVALID_BSAFILE(0, 0, "INVALID", 0); // NOLINT(cert-err58-cpp)

//******************************************************************************
// Constructors
//******************************************************************************

BsaFile::BsaFile():
        mSize(BsaFile::INVALID_BSAFILE.size()), mStartOffsetInArchive(BsaFile::INVALID_BSAFILE.startOffsetInArchive()),
        mFileName(BsaFile::INVALID_BSAFILE.fileName()), mIndex(BsaFile::INVALID_BSAFILE.index()) {
}

BsaFile::BsaFile(quint32 size,
                 qint64 startOffsetInArchive,
                 QString fileName,
                 quint16 index):
    mSize(size), mStartOffsetInArchive(startOffsetInArchive),
    mFileName(std::move(fileName)), mIndex(index)
{

}

//**************************************************************************
// Operators
//**************************************************************************

bool BsaFile::operator ==(const BsaFile &bsaFile) const
{
    return mFileName == bsaFile.mFileName;
}

bool BsaFile::operator !=(const BsaFile &bsaFile) const
{
    return !(this->operator==(bsaFile));
}

//**************************************************************************
// Methods
//**************************************************************************
QString BsaFile::getExtension() const {
    int idx = mFileName.lastIndexOf('.');
    if (idx == -1 || idx == mFileName.length()) {
        return {};
    }
    else {
        return mFileName.mid(idx + 1);
    }
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