#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "BsaFile.h"
#include "../error/Status.h"
#include "memory"
#include <QVector>
#include <QDataStream>
#include <QFile>

/**
 * @brief Describe a BSA archive
 *
 * The archive is built on the following pattern:
 * - File number: 2 bytes (max file number: 65 535)
 * - Files data put directly one after an other
 * - File table: 18 bytes for each
 *   - 14 bytes for the name (max character number for file name: 13)
 *   - 4 bytes for the file size (max file size: 4 294 967 295 bytes)
 *
 * Datas are written in little endian
 */
class BsaArchive
{
public:
    //**************************************************************************
    // Constructors/Destructor
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
    QString getArchiveFileName() const;
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
     * @return the status of the operation: -1 if failure, 0 if success
     */
    Status openArchive(const QString &filePath);
    /**
     * @brief close this archive and restore state to a not opened archive
     * @return the status of the operation: -1 if failure (example: archive
     * not opened), 0 if success
     */
    Status closeArchive();
    /**
     * @brief retourne the archive data for the file
     * @param file the file to read, It should have at least the index set
     * @return the file data. If an error occured and the data can not be read
     * an empty vector is returned. The same happens if the archive is not
     * opened or the file is new
     */
    QVector<char> getFileData(const BsaFile &file);
    /**
     * @brief extract a file.
     * Does not work for new file and extract the archive file, not the updated
     * one in case of an updated file
     * @param destinationFolder destination folder of the file
     * @param file file to extract. It should have at least the index set
     * @return the status of the operation: -1 if failure, 0 if success
     */
    Status extractFile(const QString &destinationFolder,
                      const BsaFile &file);
    /**
     * @brief update a file by a new one
     *
     * An invalid file is returned in case a wrong file provided, for example
     * if the index is invalid. This is also the case if the update file does
     * not exist
     * @param updateFilePath path the new file
     * @param file file to update. It should have at least the index set
     * @return the file with its state updated
     */
    BsaFile updateFile(const QString &updateFilePath,
                    const BsaFile &file);
    /**
     * @brief delete a file
     *
     * An invalid file is returned in case a wrong file provided, for example
     * if the index is invalid.
     * @param file file to delete. It should have at least the index set
     * @return the file with its state updated
     */
    BsaFile deleteFile(const BsaFile &file);
    /**
     * @brief addFile add a new file to the archive
     * @param filePath path the new file
     * @return the file created
     */
    BsaFile addFile(const QString &filePath);
    /**
     * @brief cancel the delete operation pending on a file
     *
     * An invalid file is returned in case a wrong file provided, for example
     * if the index is invalid. Noting is done if the file is not to delete
     * @param file the file for which the operation has to be cancel
     * @return the file with its state updated
     */
    BsaFile cancelDeleteFile(const BsaFile &file);
    /**
     * @brief cancel the update operation pending on a file
     *
     * An invalid file is returned in case a wrong file provided, for example
     * if the index is invalid. Noting is done if the file is not update
     * @param file the file for which the operation has to be cancel
     * @return the file with its state updated
     */
    BsaFile cancelUpdateFile(const BsaFile &file);
    /**
     * @brief create a new empty archive
     * @return the status of the operation: -1 if failure (example: archive
     * already opened), 0 if success
     */
    Status createNewArchive();
    /**
     * @brief save the archive to the given file path
     * @param filePath path to the save file
     * @return the status of the operation: -1 if failure, 0 if success
     */
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

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief check and return error if file is out of the archive file table,
     * the archive is not opened and optionally if the file is new
     * @param file the file to check
     * @param checkNew check for not new file if true
     * @return 0 if no error cases found, -1 otherwise
     */
    Status verifyIndexOpenOrNewErrors(const BsaFile &file,
                                      bool checkNew = false);
    /**
     * @brief update the mModified attribute
     */
    void updateIsModified();
};

#endif // ARCHIVE_H
