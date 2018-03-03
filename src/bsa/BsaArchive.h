#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "BsaFile.h"
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
    /**
     * @brief Archive constructor
     *
     * This constructor will try to open the given archive
     * @param filePath the filepath to the archive
     */
    //TODO implements
    BsaArchive(const std::string &filePath);

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
     * @return 0 all good, 1 bad bsa format or corrupted bsa, 2 error while opening
     */
    // TODO implements
    int openArchive(const std::string &filePath);
    //FIXME add other methods needed

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief complete archive path with filename
     */
    std::string archiveFilePath;
    /**
     * @brief fileNumber (bytes 1 to 2 of the archive)
     */
    uint16_t fileNumber;
    /**
     * @brief archive total size
     */
    size_t bsaSize;
    /**
     * @brief archive total size including all current modifications
     */
    int modifiedSize;
    /**
     * @brief List of the archive files
     */
    std::vector<BsaFile> files;
    /**
     * @brief file stream reading the archive file
     */
    std::ifstream archiveReadingStream;
    /**
     * @brief Map allowing the retreave a file index from its filename
     */
    std::map<std::string, std::string> fileTypeMap;
    /**
     * @brief true if an archive is opened
     */
    bool opened;
    /**
     * @brief true if the opened archive has been modified
     */
    bool modified;
};

#endif // ARCHIVE_H
