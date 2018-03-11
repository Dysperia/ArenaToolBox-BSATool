#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "BsaFile.h"
#include "../error/Status.h"
#include <QVector>
#include <QDataStream>
#include <QFile>

/**
 * @brief Describe a BSA archive
 *
 * The archive is built on the following pattern:
 * - File number: 2 bytes
 * - Files data put directly one after an other
 * - File table: 18 bytes for each
 *   - 14 bytes for the name
 *   - 4 bytes for the file size
 *
 * All data are written in little endian
 */
class BsaArchive
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Archive constructor
     */
    BsaArchive();
    /**
     * @brief Archive destructor
     */
    ~BsaArchive();

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    QString getArchiveFilePath() const;
    quint16 getFileNumber() const;
    qint64 getSize() const;
    qint64 getModifiedSize() const;
    QVector<BsaFile> getFiles() const;
    bool isOpened() const;
    bool isModified() const;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief open the given archive
     * @param filePath the filepath to the archive
     * @return the status of the operation :
     * -1 if failed
     * 0 if success
     */
    Status openArchive(const QString &filePath);
    /**
     * @brief close this archive and restore state to a not opened archive
     */
    void closeArchive();
    /**
     * @brief extract a file
     * @param destinationFolder destination folder of the file
     * @param file file to extract
     * @return the status of the operation :
     *
     */
    Status extractFile(const QString &destinationFolder,
                      const BsaFile &file);
    BsaFile updateFile(const QString &updateFilePath,
                    const BsaFile &file);
    BsaFile deleteFile(const BsaFile &file);
    BsaFile addFile(const QString &filePath);
    BsaFile cancelDeleteFile(const BsaFile &file);
    BsaFile cancelUpdateFile(const BsaFile &file);
    void createNewArchive();
    Status saveArchive(const QString &filePath);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief complete archive path with filename
     */
    QFile mArchiveFile{};
    /**
     * @brief fileNumber (bytes 1 to 2 of the archive)
     */
    quint16 mFileNumber{0};
    /**
     * @brief archive total size
     */
    qint64 mSize{0};
    /**
     * @brief archive total size including all current modifications
     */
    qint64 mModifiedSize{0};
    /**
     * @brief List of the archive files
     */
    QVector<BsaFile> mFiles{};
    /**
     * @brief file stream reading the archive file
     */
    QDataStream mReadingStream{};
    /**
     * @brief true if an archive is opened
     */
    bool mOpened{false};
    /**
     * @brief true if the opened archive has been modified
     */
    bool mModified{false};
};

#endif // ARCHIVE_H
