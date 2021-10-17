#ifndef BSAFILE_HPP
#define BSAFILE_HPP

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>

class BSAFile
{
private:

    std::string archiveFilePath;
    std::string archiveFileName;
    uint16_t fileNumber; // first 2 bytes of the archive
    size_t bsaSize;
    int modifiedSize;

    // File name buffer
    std::vector<char> fileNameBuffer;

    // One entry of the file table
    struct FileStruct
    {
        bool isNew;
        bool isDeleted;
        bool isUpdated;
        std::string updateFilePath;
        std::string newFilePath;
        std::string newFileName;
        uint32_t fileSize;
        int updatedSize;
        size_t fileOffset;
        const char *fileName;
    };
    std::vector<FileStruct> fileTable;

    // Map for getting index from filename
    std::map<std::string, int> fileMap;

    bool isOpened;
    bool isModified;
    bool isSaved;

    // BSA Archive file input stream
    std::ifstream bsaIStream;

    BSAFile();

public:

    static BSAFile *getInstance();

    int openArchive(const std::string &filePath);

    std::string getArchiveFileName() const;

    std::ifstream *getStream();

    std::string getArchiveFilePath() const;

    bool getIsOpened() const;

    bool getIsModified() const;

    bool getIsSaved() const;

    uint16_t getFileNumber() const;

    size_t getArchiveSize() const;

    std::string getFileName(int index) const;

    size_t getFileSize(int index) const;

    uint32_t getFileOffset(int index) const;

    // Return an upper file extension
    std::string getFileExtension(int index);

    // Get the index number of a given file name
    int getIndex(const std::string &fileName);

    // Clear bsa data to prepare a new bsa load or a new creation
    void clear();

    // Extract file
    int extractFile(const std::string &filePath, int index);

    // Update a file in archive data with an other
    void updateFile(int index, const std::string &filePath);

    // Delete a file in archive data
    void deleteFile(int index);

    // Add a file to archive data
    void addFile(const std::string &filePath);

    // Cancel delete file to archive data
    void cancelDeleteFile(int index);

    // Cancel update file to archive data
    void cancelUpdateFile(int index);

    // Return archive modified size
    int getArchiveModifiedSize();

    // Return true if file is to be deleted
    bool getIsFileDelete(int index);

    // Return true if file is new
    bool getIsFileNew(int index);

    // Return true if file is updated
    bool getIsFileUpdated(int index);

    // Return the updated file size or 0
    int getUpdatedFileSize(int index);

    // create a new empty bsa archive
    void createArchive();

    // Return the file path of a new file, or empty string
    std::string getNewFilePath(int index);

    // Return the file path of an updated file, or empty string
    std::string getUpdatedFilePath(int index);

    // Save the current data to a BSA archive. Don't forget to reload the saved archive in order to update all the data and have a cleaned window
    int saveArchiveToBSA(const std::string &filePath);
};

#endif // BSAFILE_HPP
