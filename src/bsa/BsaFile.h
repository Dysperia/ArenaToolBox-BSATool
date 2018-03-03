#ifndef FILE_H
#define FILE_H

#include <string>

/**
 * @brief The File class
 *
 * Describe the
 */
class BsaFile
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief File construtor
     * @param size the file size
     * @param startOffsetInArchive offset at which the data of this file starts
     * in the archive
     * @param fileName the file name
     */
    BsaFile(const uint32_t &size,
         const size_t &startOffsetInArchive,
         const std::string fileName);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    uint32_t size() const;
    size_t startOffsetInArchive() const;
    const std::string fileName() const;
    bool isNew() const;
    void setIsNew(bool isNew);
    bool toDeleted() const;
    void setToDeleted(bool toDeleted);
    bool updated() const;
    void setUpdated(bool updated);
    int updateFileSize() const;
    void setUpdateFileSize(int updateFileSize);
    std::string updateFilePath() const;
    void setUpdateFilePath(const std::string &updateFilePath);
    std::string newFilePath() const;
    void setNewFilePath(const std::string &newFilePath);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief file size
     */
    const uint32_t mSize;
    /**
     * @brief start offset of the file data in archive
     */
    const size_t mStartOffsetInArchive;
    /**
     * @brief file name
     */
    const std::string mFileName;
    /**
     * @brief true if the file is new and to add to the archive
     */
    bool mIsNew;
    /**
     * @brief true if the file is to be remove of the archive
     */
    bool mToDeleted;
    /**
     * @brief true if the file is to be updated with a new version
     */
    bool mUpdated;
    /**
     * @brief the size of the update file
     */
    int mUpdateFileSize;
    /**
     * @brief the complete path to the update file
     */
    std::string mUpdateFilePath;
    /**
     * @brief the complete path to the new file
     */
    std::string mNewFilePath;
};

#endif // ENTRY_H
