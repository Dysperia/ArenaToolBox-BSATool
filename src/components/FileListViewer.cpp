#include "FileListViewer.h"
#include <QLabel>

FileListViewer::FileListViewer()
{
    QHBoxLayout *filterLayout = new QHBoxLayout;
    QLabel *filterLabel = new QLabel("Filter");
    mFileExtensionFilter = new QComboBox;
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(mFileExtensionFilter);
    this->addLayout(filterLayout);

    mArchiveFileList = new QListWidget;
    this->addWidget(mArchiveFileList);

    // Connecting filter mecanic
    connect(mFileExtensionFilter, SIGNAL(activated(QString)), SLOT(updateFileListFromFilterSlot(QString)));

   // Passing throw signal from internal widget
    connect(mArchiveFileList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
}

void FileListViewer::updateFileListFromFilterSlot(QString filter) {

}
