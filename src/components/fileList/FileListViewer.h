#ifndef BSATOOL_FILELISTVIEWER_H
#define BSATOOL_FILELISTVIEWER_H

#include <bsa/BsaFile.h>

#include <QVBoxLayout>
#include <QComboBox>
#include <QListWidget>

/**
 * @brief Widget using a QListWidget to display a list of the bsa archive files.
 * A QComboBox is added to filter the displayed file according to the files
 * possible extensions
 */
class FileListViewer : public QWidget
{
    Q_OBJECT
signals:
    void currentFileItemChanged(BsaFile current, BsaFile previous);

public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Default constructor
     * @param parent of the FileListViewerWithFilterWidget
     */
    explicit FileListViewer(QWidget *parent = Q_NULLPTR);

public slots:
    /**
     * @brief update the list from the bsa file list
     * @param fileList list of the bsa archive files
     */
    void updateViewFromFileList(const QVector<BsaFile>& fileList);
    /**
     * @brief emit a signal to notify a change in the current item in the file list
     * @param current item
     * @param previous item
     */
    void emitCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief the file extension filter
     */
    QComboBox *mFileExtensionFilter;
    /**
     * @brief the file list widget
     */
    QListWidget *mFileList;

private slots:
    /**
     * @brief update the view when the filter changed, hidding any file which
     * has a extension not matching the current selected filter
     *
     * @param filter the current selected file extension
     */
    void updateViewFromFilterChange(const QString& filter);

};

#endif // BSATOOL_FILELISTVIEWER_H
