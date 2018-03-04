#ifndef FILE_H
#define FILE_H

#include <string>

/**
 * @brief The File class
 *
 * Describe an archive file
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
         const std::string fileName,
         const size_t &index);
    /**
     * @brief copy construtor
     */
    BsaFile(const BsaFile &bsaFile);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    uint32_t size() const;
    size_t startOffsetInArchive() const;
    std::string fileName() const;
    size_t index() const;
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
    const uint32_t mSize{0};
    /**
     * @brief start offset of the file data in archive
     */
    const size_t mStartOffsetInArchive{0};
    /**
     * @brief file name
     */
    const std::string mFileName{};
    /**
     * @brief index of the in archive
     */
    const size_t mIndex{0};
    /**
     * @brief true if the file is new and to add to the archive
     */
    bool mIsNew{false};
    /**
     * @brief true if the file is to be remove of the archive
     */
    bool mToDeleted{false};
    /**
     * @brief true if the file is to be updated with a new version
     */
    bool mUpdated{false};
    /**
     * @brief the size of the update file
     */
    int mUpdateFileSize{0};
    /**
     * @brief the complete path to the update file
     */
    std::string mUpdateFilePath{};
    /**
     * @brief the complete path to the new file
     */
    std::string mNewFilePath{};
};

#endif // ENTRY_H
