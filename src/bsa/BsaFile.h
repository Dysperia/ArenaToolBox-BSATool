#ifndef BSATOOL_FILE_H
#define BSATOOL_FILE_H

#include <QString>

/**
 * @brief The File class
 *
 * Describe an archive file. A file with an offset of zero indicate an invalid
 * file since the minimum is 2 (bsa archive begins by two bytes for file number)
 */
class BsaFile
{
public:
    //**************************************************************************
    // Statics
    //**************************************************************************

    /**
     * @brief Invalid bsaFile used to return an error. Invalid because an offset
     * of 0 is impossible in a bsa archive: 2 bytes minimum
     */
    const static BsaFile INVALID_BSAFILE;

    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief default constructor (used by QVector)
     */
    BsaFile();
    /**
     * @brief File constructor
     * @param size the file size
     * @param startOffsetInArchive offset at which the data of this file starts
     * in the archive
     * @param fileName the file name
     */
    BsaFile(quint32 size,
         qint64 startOffsetInArchive,
         QString fileName,
         quint16 index);
    /**
     * @brief copy constructor
     */
    BsaFile(const BsaFile &bsaFile) = default;

    //**************************************************************************
    // Operators
    //**************************************************************************
    /**
     * @brief equal operator
     *
     * Two files are considered equal if their filenames are the same
     */
    bool operator ==(const BsaFile &bsaFile) const;
    /**
     * @brief not equal operator
     *
     * Two files are considered not equal if their filenames are different
     */
    bool operator !=(const BsaFile &bsaFile) const;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief return the file name extension
     *
     * @return return this file name extension or an empty string if no '.' character is found in the file name
     */
    [[nodiscard]]QString getExtension() const;

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    [[nodiscard]]quint32 size() const;

    [[nodiscard]]qint64 startOffsetInArchive() const;

    [[nodiscard]]QString fileName() const;

    [[nodiscard]]quint16 index() const;

    [[nodiscard]]bool isNew() const;
    void setIsNew(bool isNew);

    [[nodiscard]]bool toDelete() const;
    void setToDelete(bool toDelete);

    [[nodiscard]]bool updated() const;
    void setUpdated(bool updated);

    [[nodiscard]]quint32 updateFileSize() const;
    void setUpdateFileSize(quint32 updateFileSize);

    [[nodiscard]]QString updateFilePath() const;
    void setUpdateFilePath(const QString &updateFilePath);

    [[nodiscard]]QString newFilePath() const;
    void setNewFilePath(const QString &newFilePath);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief file size
     */
    quint32 mSize{0};
    /**
     * @brief start offset of the file data in archive
     */
    qint64 mStartOffsetInArchive{0};
    /**
     * @brief file name
     */
    QString mFileName{};
    /**
     * @brief index of the in archive (starts at zero)
     */
    quint16 mIndex{0};
    /**
     * @brief true if the file is new and to add to the archive
     */
    bool mIsNew{false};
    /**
     * @brief true if the file is to be remove of the archive
     */
    bool mToDelete{false};
    /**
     * @brief true if the file is to be updated with a new version
     */
    bool mUpdated{false};
    /**
     * @brief the size of the update file
     */
    quint32 mUpdateFileSize{0};
    /**
     * @brief the complete path to the update file
     */
    QString mUpdateFilePath{};
    /**
     * @brief the complete path to the new file
     */
    QString mNewFilePath{};
};

#endif // BSATOOL_FILE_H
