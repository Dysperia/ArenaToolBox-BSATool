#ifndef FILELISTVIEWER_H
#define FILELISTVIEWER_H

#include <QVBoxLayout>
#include <QComboBox>
#include <QListWidget>

class FileListViewer : public QVBoxLayout
{
    Q_OBJECT
signals:
    void currentItemChanged(QListWidgetItem*,QListWidgetItem*);

public:
    FileListViewer();

public slots:

private:
    QComboBox *mFileExtensionFilter;
    QListWidget *mArchiveFileList;

private slots:
    void updateFileListFromFilterSlot(QString filter);

};

#endif // FILELISTVIEWER_H
