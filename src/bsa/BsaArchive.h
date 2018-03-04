#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "BsaFile.h"
#include "../error/Error.h"
#include <vector>
#include <map>
#include <fstream>

/**
 * @brief Describe a BSA archive
 *
 * The archive is built on the following pattern:
 * - File number: 2 bytes
 * - Files data put directly one after an other
 * - File table: 18 bytes for each
 *   - 14 bytes for the name
 *   - 4 bytes for the file size
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

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    std::string getArchiveFilePath() const;
    uint16_t getFileNumber() const;
    size_t getBsaSize() const;
    int getModifiedSize() const;
    std::vector<BsaFile> getFiles() const;
    bool isOpened() const;
    bool isModified() const;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief open the given archive
     * @param filePath the filepath to the archive
     * @return the status of the operation
     */
    Error openArchive(const std::string &filePath);
    void clear();
    Error extractFile(const std::string &destinationFolder,
                      const BsaFile &file);
    BsaFile updateFile(const std::string &updateFilePath,
                    const BsaFile &file);
    BsaFile deleteFile(const BsaFile &file);
    BsaFile addFile(const std::string &filePath);
    BsaFile cancelDeleteFile(const BsaFile &file);
    BsaFile cancelUpdateFile(const BsaFile &file);
    void createNewArchive();
    Error saveArchive(const std::string &filePath);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief complete archive path with filename
     */
    std::string mArchiveFilePath{};
    /**
     * @brief fileNumber (bytes 1 to 2 of the archive)
     */
    uint16_t mFileNumber{0};
    /**
     * @brief archive total size
     */
    size_t mBsaSize{0};
    /**
     * @brief archive total size including all current modifications
     */
    int mModifiedSize{0};
    /**
     * @brief List of the archive files
     */
    std::vector<BsaFile> mFiles{};
    /**
     * @brief file stream reading the archive file
     */
    std::ifstream mArchiveReadingStream{};
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
