#include "BsaArchive.h"

#include <QtConcurrent/QtConcurrent>
#include "functional"

//******************************************************************************
// Constructors
//******************************************************************************
BsaArchive::BsaArchive()
{
    mReadingStream.setByteOrder(QDataStream::LittleEndian);
}

BsaArchive::~BsaArchive()
{
    mArchiveFile.close();
}

//******************************************************************************
// Getters/setters
//******************************************************************************
QString BsaArchive::getArchiveFilePath() const
{
    return mArchiveFile.fileName();
}

quint16 BsaArchive::getFileNumber() const
{
    return mFileNumber;
}

qint64 BsaArchive::getSize() const
{
    return mSize;
}

qint64 BsaArchive::getModifiedSize() const
{
    return mModifiedSize;
}

QVector<BsaFile> BsaArchive::getFiles() const
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
Status BsaArchive::openArchive(const QString &filePath)
{
    if (mOpened) {
        return Status(-1, QStringLiteral("An archive is already opened"));
    }
    mArchiveFile.setFileName(filePath);
    if (!mArchiveFile.open(QIODevice::ReadOnly)) {
        return Status(-1, QString("Could not open the file in read mode : %1")
                     .arg(filePath));
    }
    // Getting total file size
    mSize = mArchiveFile.size();
    mModifiedSize = mSize;
    mReadingStream.setDevice(&mArchiveFile);
    // Reading file number
    mArchiveFile.seek(0);
    mReadingStream >> mFileNumber;
    // Reading files name and size
    quint16 fileTableSize = 18*mFileNumber;
    mArchiveFile.seek(mSize - fileTableSize);
    quint32 offset = 2;
    char name[14];
    quint32 size = 0;
    for (quint16 i(0); i < mFileNumber; i++) {
        if (mArchiveFile.atEnd()) {
            return Status(-1, QString("Reached end of file while reading infos of file %1 of %2")
                         .arg(i+1).arg(mFileNumber));
        }
        if (mReadingStream.readRawData(&name[0], 14) < 14) {
            return Status(-1, QString("Could not read file name of file %1 of %2")
                         .arg(i+1).arg(mFileNumber));
        }
        mReadingStream >> size;
        mFiles.append(BsaFile(size, offset, QString(&name[0]), i));
        offset += size;
    }
    // Checking archive size and integrity
    auto sizeReduce = [](qint64 &result, const qint64 &current) -> void {result += current;};
    qint64 totalSizeFromFiles = QtConcurrent::blockingMappedReduced<qint64>(
                                    mFiles, &BsaFile::size, sizeReduce);
    totalSizeFromFiles += 2 + fileTableSize;
    if (totalSizeFromFiles != mSize) {
        return Status(-1, QString("The archive seems corrupted (actual size : %1, expected size : %2")
                     .arg(mSize).arg(totalSizeFromFiles));
    }
    // Archive has been read and ok -> opened
    mOpened = true;
    return Status(0);
}

void BsaArchive::closeArchive()
{
    if (mArchiveFile.openMode() != QIODevice::NotOpen)
    {
        mArchiveFile.close();
    }
    mReadingStream.setDevice(nullptr);
    mOpened = false;
    mModified = false;
    mSize = 0;
    mModifiedSize = 0;
    mFileNumber = 0;
    mFiles.clear();
}

Status BsaArchive::extractFile(const QString &destinationFolder, const BsaFile &file)
{
    //TODO implements extractFile
    return Status(1);
}

BsaFile BsaArchive::updateFile(const QString &updateFilePath, const BsaFile &file)
{
    //TODO implements updateFile
    return BsaFile(0,0,"",0);
}

BsaFile BsaArchive::deleteFile(const BsaFile &file)
{
    //TODO implements deleteFile
    return BsaFile(0,0,"",0);
}

BsaFile BsaArchive::addFile(const QString &filePath)
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

Status BsaArchive::saveArchive(const QString &filePath)
{
    //TODO implements saveArchive
    return Status(1);
}
