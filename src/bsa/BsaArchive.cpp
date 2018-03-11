#include "BsaArchive.h"

#include <QtConcurrent/QtConcurrent>
#include "functional"

const BsaFile INVALID_BSAFILE(0, 0, "", 0); // offset of zero is impossible

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
    // Reading files name and size from file table
    quint16 fileTableSize = 18*mFileNumber;
    mArchiveFile.seek(mSize - fileTableSize);
    quint32 offset = 2;
    char name[14];
    quint32 size = 0;
    mFiles.resize(mFileNumber);
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

Status BsaArchive::verifyIndexOpenOrNewErrors(const BsaFile &file,
                                              bool checkNew) {
    if (!mOpened) {
        return Status(-1, QStringLiteral("The archive is not opened"));
    }
    if (file.index() >= mFileNumber) {
        return Status(-1, QStringLiteral("The file index is out of the archive"));
    }
    if (checkNew && mFiles.at(file.index()).isNew()) {
        return Status(-1, QStringLiteral("The operation cannot be performed for new files"));
    }
    return Status(0);
}

QVector<char> BsaArchive::getFileData(const BsaFile &file)
{
    Status status = verifyIndexOpenOrNewErrors(file, true);
    if (status.status() < 0) {
        return QVector<char>(0);
    }
    BsaFile internFile = mFiles.at(file.index());
    // seek error
    if (!mArchiveFile.seek(internFile.startOffsetInArchive())) {
        return QVector<char>(0);
    }
    QVector<char> data(internFile.size());
    size_t bytesRead = mReadingStream.readRawData(data.data(), internFile.size());
    return bytesRead == internFile.size() ? data : QVector<char>(0);
}

Status BsaArchive::extractFile(const QString &destinationFolder, const BsaFile &file)
{
    Status status = verifyIndexOpenOrNewErrors(file, true);
    if (status.status() < 0) {
        return status;
    }
    BsaFile internFile = mFiles.at(file.index());
    QFile saveFile(destinationFolder + QDir::separator() + internFile.fileName());
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return Status(-1, QString("Could not open the file in write mode : %1")
                     .arg(saveFile.fileName()));
    }
    QDataStream writeStream(&saveFile);
    QVector<char> data = getFileData(internFile);
    size_t bytesRead = data.size();
    // data size too small
    if (bytesRead < internFile.size()) {
        return Status(-1, QString("To few data read from the archive, actual :%1, expected : %2")
                     .arg(data.size()).arg(internFile.size()));
    }
    size_t bytesWritten = writeStream.writeRawData(data.constData(), data.size());
    if (bytesWritten < internFile.size()) {
        return Status(-1, QString("Only %1 bytes of %2 written to file %3")
                      .arg(bytesWritten).arg(internFile.size())
                      .arg(saveFile.fileName()));
    }
    return Status(0);
}

BsaFile BsaArchive::updateFile(const QString &updateFilePath, const BsaFile &file)
{
    Status status = verifyIndexOpenOrNewErrors(file);
    if (status.status() < 0) {
        return INVALID_BSAFILE;
    }
    // Update file should exist
    QFile updateFile(updateFilePath);
    if (!updateFile.exists()) {
        return INVALID_BSAFILE;
    }
    quint32 updateSize = updateFile.size();
    // Updating file state
    BsaFile internFile = mFiles[file.index()];
    internFile.setUpdated(true);
    internFile.setUpdateFilePath(updateFilePath);
    internFile.setUpdateFileSize(updateSize);
    return internFile;
}

BsaFile BsaArchive::deleteFile(const BsaFile &file)
{
    Status status = verifyIndexOpenOrNewErrors(file);
    if (status.status() < 0) {
        return INVALID_BSAFILE;
    }
    // Updating file state
    BsaFile internFile = mFiles[file.index()];
    internFile.setToDelete(true);
    return internFile;
}

BsaFile BsaArchive::addFile(const QString &filePath)
{
    //TODO implements addFile
    return BsaFile(0,0,"",0);
}

BsaFile BsaArchive::cancelDeleteFile(const BsaFile &file)
{
    Status status = verifyIndexOpenOrNewErrors(file);
    if (status.status() < 0) {
        return INVALID_BSAFILE;
    }
    BsaFile internFile = mFiles[file.index()];
    // Noting to be done if file not to delete
    if (!internFile.toDelete()) {
        return internFile;
    }
    // Updating file state
    internFile.setToDelete(false);
    return internFile;
}

BsaFile BsaArchive::cancelUpdateFile(const BsaFile &file)
{
    Status status = verifyIndexOpenOrNewErrors(file);
    if (status.status() < 0) {
        return INVALID_BSAFILE;
    }
    BsaFile internFile = mFiles[file.index()];
    // Noting to be done if file not updated
    if (!internFile.updated()) {
        return internFile;
    }
    // Updating file state
    internFile.setUpdated(false);
    internFile.setUpdateFilePath("");
    internFile.setUpdateFileSize(0);
    return internFile;
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
