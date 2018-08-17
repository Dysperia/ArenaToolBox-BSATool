#include "FileListViewer.h"
#include <QLabel>

const QString ALL_TYPE = QString("All types");

FileListViewer::FileListViewer()
{
    QHBoxLayout *filterLayout = new QHBoxLayout;
    QLabel *filterLabel = new QLabel("File type");
    mFileExtensionFilter = new QComboBox;
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(mFileExtensionFilter);
    this->addLayout(filterLayout);

    mFileListView = new QListWidget;
    this->addWidget(mFileListView);

    // Connecting filter mecanic
    connect(mFileExtensionFilter, SIGNAL(activated(QString)), SLOT(updateViewFromFilterChange(QString)));

   // Passing throw signal from internal widget
    connect(mFileListView, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
}

void FileListViewer::updateViewFromFilterChange(QString filter) {
    for (int i=0; i<mFileListView->count(); i++) {
        QListWidgetItem *item = mFileListView->item(i);
        item->setHidden(filter != ALL_TYPE && !item->text().toUpper().endsWith(filter));
    }
}

// Generate the file filter lists
void FileListViewer::updateViewFromFileList(QVector<BsaFile> fileList)
{
    QStringList extensions;
    mFileListView->clear();
    for (int i(0); i < fileList.size(); i++)
    {
        BsaFile file = fileList.at(i);
        int lastPointPosition = file.fileName().lastIndexOf('.');
        QString extension;
        if (lastPointPosition == -1) {
            extension = QString("No type");
        }
        else {
            extension = file.fileName().mid(lastPointPosition + 1).toUpper();
        }
        if (!extensions.contains(extension)) {
            extensions.append(extension);
        }
        mFileListView->addItem(file.fileName());
    }
    extensions.sort();
    extensions.prepend(ALL_TYPE);
    mFileExtensionFilter->addItems(extensions);
}
