#ifndef FILE_H
#define FILE_H

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
    BsaFile(const quint32 &size,
         const qint64 &startOffsetInArchive,
         const QString fileName,
         const quint16 &index);
    /**
     * @brief copy constructor
     */
    BsaFile(const BsaFile &bsaFile) = default;

    //**************************************************************************
    // Operators
    //**************************************************************************
    /**
     * @brief copy assigment operator
     */
    BsaFile &operator =(const BsaFile &bsaFile) = default;
    /**
     * @brief equals operator
     *
     * Two files are considered equal if their filename and index in archive are
     * the same
     */
    bool operator ==(const BsaFile &bsaFile) const;

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    quint32 size() const;

    qint64 startOffsetInArchive() const;

    QString fileName() const;

    quint16 index() const;

    bool isNew() const;
    void setIsNew(bool isNew);

    bool toDelete() const;
    void setToDelete(bool toDelete);

    bool updated() const;
    void setUpdated(bool updated);

    quint32 updateFileSize() const;
    void setUpdateFileSize(quint32 updateFileSize);

    QString updateFilePath() const;
    void setUpdateFilePath(const QString &updateFilePath);

    QString newFilePath() const;
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

#endif // ENTRY_H
