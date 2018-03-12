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

QString BsaArchive::getArchiveFileName() const
{
    return QFileInfo(mArchiveFile).fileName();
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

Status BsaArchive::closeArchive()
{
    if (!mOpened) {
        return Status(-1, QStringLiteral("Cannot close : archive not opened"));
    }
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
    return Status(0);
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

void BsaArchive::updateIsModified()
{
    if (mOpened) {
        for (quint16 i(0); i < mFileNumber; i++) {
            const BsaFile &internFile = mFiles.at(i);
            if (internFile.isNew() || internFile.toDelete() || internFile.updated()) {
                mModified = true;
                return;
            }
        }
        mModified = false;
    }
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
    saveFile.flush();
    saveFile.close();
    return Status(0);
}

BsaFile BsaArchive::updateFile(const QString &updateFilePath, const BsaFile &file)
{
    Status status = verifyIndexOpenOrNewErrors(file);
    if (status.status() < 0) {
        return INVALID_BSAFILE;
    }
    // Update file should exist and be readable for size
    QFile updateFile(updateFilePath);
    if (!updateFile.exists() || !updateFile.open(QIODevice::ReadOnly)) {
        return INVALID_BSAFILE;
    }
    quint32 updateSize = updateFile.size();
    updateFile.close();
    // Updating file state
    BsaFile internFile = mFiles[file.index()];
    internFile.setUpdated(true);
    internFile.setUpdateFilePath(updateFilePath);
    internFile.setUpdateFileSize(updateSize);
    // Updating archive state
    mModifiedSize -= internFile.size();
    mModifiedSize += internFile.updateFileSize();
    mModified = true;
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
    // Updating archive state
    mModifiedSize -= (internFile.updated() ? internFile.updateFileSize() : internFile.size());
    mModified = true;
    return internFile;
}

BsaFile BsaArchive::addFile(const QString &filePath)
{
    QFile newFile(filePath);
    // New file should exist and be readable for size
    if (!newFile.exists() || !newFile.open(QIODevice::ReadOnly)) {
        return INVALID_BSAFILE;
    }
    quint32 newFileSize = newFile.size();
    newFile.close();
    QString newFileName = QFileInfo(newFile).fileName();
    // Checking filename length
    if (newFileName.size() > 13) {
        return INVALID_BSAFILE;
    }
    BsaFile newBsaFile(newFileSize, 2, newFileName, mFileNumber);
    newBsaFile.setIsNew(true);
    newBsaFile.setNewFilePath(filePath);
    // Updating archive state
    mFiles.append(newBsaFile);
    mFileNumber++;
    mModifiedSize += newFileSize;
    mModified = true;
    return newBsaFile;
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
    // Updating archive state
    mModifiedSize += (internFile.updated() ? internFile.updateFileSize() : internFile.size());
    updateIsModified();
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
    // Updating archive state
    mModifiedSize += internFile.size();
    mModifiedSize -= internFile.updateFileSize();
    updateIsModified();
    return internFile;
}

Status BsaArchive::createNewArchive()
{
    if (mOpened) {
        return Status(-1, QStringLiteral("Cannot create archive: already opened"));
    }
    // init empty archive data
    mArchiveFile.setFileName("");
    mFileNumber = 0;
    mFiles.clear();
    mModified = false;
    mModifiedSize = 0;
    mOpened = true;
    mReadingStream.setDevice(nullptr);
    mSize = 0;
    return Status(0);
}

Status BsaArchive::saveArchive(const QString &filePath)
{
    if (!mOpened) {
        return Status(-1, QStringLiteral("Cannot save archive: not opened"));
    }
    QFile saveFile(filePath + ".tmp");
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return Status(-1, QString("Cannot save archive: could not write temporary file %1")
                      .arg(saveFile.fileName()));
    }
    QDataStream saveStream(&saveFile);
    // Count number of file to write in archive
    quint16 nbFileToSave = mFileNumber;
    for (quint16 i(0); i<mFileNumber; i++) {
        if (mFiles.at(i).toDelete()) {
            nbFileToSave--;
        }
    }
    // Writing header
    saveStream << nbFileToSave;
    // Writing files data
    size_t totalFileSize(0);
    QVector<char> fileData;
    for (quint16 i(0); i<mFileNumber; i++) {
        // To skip deleted file
        if (!mFiles.at(i).toDelete()) {
            const BsaFile &file = mFiles.at(i);
            quint32 dataSize(0);
            // Reading file data for updated or new
            if (file.updated() || file.isNew()) {
                QFile externFile(file.updated() ? file.updateFilePath() : file.newFilePath());
                quint32 dataSize = file.updated() ? file.updateFileSize() : file.size();
                // Error opening file
                if (!externFile.open(QIODevice::ReadOnly)) {
                    saveFile.close();
                    saveFile.remove();
                    return Status(-1, QString("Error while reading file %1")
                                  .arg(externFile.fileName()));
                }
                QDataStream externStream(&externFile);
                fileData.resize(dataSize);
                size_t bytesRead = externStream.readRawData(fileData.data(),
                                                            dataSize);
                // Error reading data
                if (bytesRead != dataSize) {
                    saveFile.close();
                    saveFile.remove();
                    return Status(-1, QString("Error while reading file %1. Got only %2 bytes of %3")
                                  .arg(externFile.fileName()).arg(bytesRead)
                                  .arg(dataSize));
                }
                externFile.close();
            }
            // reading data from archive
            else {
                if (!mArchiveFile.seek(file.startOffsetInArchive())) {
                    saveFile.close();
                    saveFile.remove();
                    return Status(-1, QString("Error while reading archive data for file %1")
                                  .arg(file.fileName()));
                }
                dataSize = file.size();
                fileData.resize(dataSize);
                size_t bytesRead = mReadingStream.readRawData(fileData.data(), dataSize);
                // Error reading data
                if (bytesRead != dataSize) {
                    saveFile.close();
                    saveFile.remove();
                    return Status(-1, QString("Error while archive data for file %1. Got only %2 bytes of %3")
                                  .arg(file.fileName()).arg(bytesRead)
                                  .arg(dataSize));
                }
            }
            // Writing file data
            size_t bytesWritten = saveStream.writeRawData(fileData.data(),
                                                          dataSize);
            // Error writing data
            if (bytesWritten != dataSize) {
                saveFile.close();
                saveFile.remove();
                return Status(-1, QString("Error while writing data for file %1. Got only %2 bytes of %3")
                              .arg(file.fileName()).arg(bytesWritten)
                              .arg(dataSize));
            }
            totalFileSize += bytesWritten;
        }
    }
    // Writing file table
    char nullString[14] = {0};
    for (quint16 i(0); i<mFileNumber; i++) {
        // To skip deleted file
        const BsaFile &file = mFiles.at(i);
        if (!file.toDelete()) {
            // Writing file name and padding name to 14 bytes
            int nameBytes = saveStream.writeRawData(file.fileName().toStdString().c_str(),
                                    file.fileName().size());
            int paddingBytes = saveStream.writeRawData(&nullString[0],
                                    14 - file.fileName().size());
            // Error writing data
            if (nameBytes + paddingBytes != 14) {
                saveFile.close();
                saveFile.remove();
                return Status(-1, QString("Error while writing data in file table for file %1")
                              .arg(file.fileName()));
            }
            // Writing file size
            saveStream << (file.updated() ? file.updateFileSize() : file.size());
        }
    }
    // Flushing and closing
    saveFile.flush();
    saveFile.close();
    // Checking temporary saved archive integrity before writing final file
    size_t expectedSize = 2 + totalFileSize + nbFileToSave*18;
    saveFile.open(QIODevice::ReadOnly);
    size_t savedSize = saveFile.size();
    saveFile.close();
    if (expectedSize != savedSize) {
        saveFile.remove();
        return Status(-1, QString("Archive not properly saved: saved archive size: %1, expected: %2")
                      .arg(savedSize).arg(expectedSize));
    }
    // Writing final file
    QFile finalFile(filePath);
    // Trying to delete existing if exists
    if (finalFile.exists()) {
        if (!finalFile.remove()) {
            return Status(-1, QString("Could not delete existing file %1. Saved archive can be found at %2")
                          .arg(finalFile.fileName()).arg(saveFile.fileName()));
        }
    }
    // Renaming temporary to final file
    if (!saveFile.rename(finalFile.fileName())) {
        return Status(-1, QString("Could not rename temporary saved archive %1 to %2. Saved archive can be found at %1")
                      .arg(saveFile.fileName()).arg(finalFile.fileName()));
    }
    return Status(0);
}
