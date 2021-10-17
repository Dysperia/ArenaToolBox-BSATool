#ifndef BSATOOL_FILELISTVIEWERITEM_H
#define BSATOOL_FILELISTVIEWERITEM_H

#include <bsa/BsaFile.h>

#include <QListWidgetItem>

/**
 * @brief QListWidgetItem containing the associated bsa file
 */
class FileListViewerItem : public QListWidgetItem
{
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Default constructor
     *
     * build the item without associated file
     */
    explicit FileListViewerItem(QListWidget *parent = Q_NULLPTR);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    [[nodiscard]]BsaFile bsaFile() const;
    void setBsaFile(const BsaFile &bsaFile);

private:
    /**
     * @brief Bsa file associated with this item
     */
    BsaFile mBsaFile;
};

#endif // BSATOOL_FILELISTVIEWERITEM_H
