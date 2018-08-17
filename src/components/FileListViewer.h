#ifndef FILELISTVIEWER_H
#define FILELISTVIEWER_H

#include "../bsa/BsaFile.h"

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
    void updateViewFromFileList(QVector<BsaFile> fileListr);

private:
    QComboBox *mFileExtensionFilter;
    QListWidget *mFileListView;

private slots:
    void updateViewFromFilterChange(QString filter);

};

#endif // FILELISTVIEWER_H
