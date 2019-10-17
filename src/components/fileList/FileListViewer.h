#ifndef FILELISTVIEWER_H
#define FILELISTVIEWER_H

#include "bsa/BsaFile.h"

#include <QVBoxLayout>
#include <QComboBox>
#include <QListWidget>

/**
 * @brief Widget using a QListWidget to display a list of the bsa archive files.
 * A QComboBox is added to filter the displayed file according to the files
 * possible extensions
 */
class FileListViewer : public QListWidget
{
    Q_OBJECT
signals:

public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Default constructor
     * @param parent of the FileListViewerWithFilterWidget
     */
    explicit FileListViewer(QWidget *parent = Q_NULLPTR);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    QVBoxLayout *fileListViewerWithFilterWidget() const;

public slots:
    /**
     * @brief update the list from the bsa file list
     * @param fileList list of the bsa archive files
     */
    void updateViewFromFileList(QVector<BsaFile> fileList);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief the file extension filter
     */
    QComboBox *mFileExtensionFilter;

    /**
     * @brief the widget actually displaying the file list
     */
    QVBoxLayout *mFileListViewerWithFilterWidget;

private slots:
    /**
     * @brief update the view when the filter changed, hidding any file which
     * has a extension not matching the current selected filter
     *
     * @param filter the current selected file extension
     */
    void updateViewFromFilterChange(QString filter);

};

#endif // FILELISTVIEWER_H
