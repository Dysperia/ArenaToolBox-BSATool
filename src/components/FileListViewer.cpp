#include "FileListViewer.h"
#include <QLabel>

//******************************************************************************
// Statics
//******************************************************************************
const QString ALL_TYPE = QString("All types");

//******************************************************************************
// Constructors
//******************************************************************************
FileListViewer::FileListViewer(QWidget *parent)
{
    // Building full widget
    QHBoxLayout *filterLayout = new QHBoxLayout;
    QLabel *filterLabel = new QLabel("File type");
    mFileExtensionFilter = new QComboBox;
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(mFileExtensionFilter);
    mFileListViewerWithFilterWidget = new QVBoxLayout(parent);
    mFileListViewerWithFilterWidget->addLayout(filterLayout);
    mFileListViewerWithFilterWidget->addWidget(this);

    // Connecting filter mecanic
    connect(mFileExtensionFilter, SIGNAL(activated(QString)), SLOT(updateViewFromFilterChange(QString)));
}

//******************************************************************************
// Getters/setters
//******************************************************************************
QVBoxLayout *FileListViewer::fileListViewerWithFilterWidget() const
{
    return mFileListViewerWithFilterWidget;
}

//**************************************************************************
// Methods
//**************************************************************************
void FileListViewer::updateViewFromFilterChange(QString filter) {
    for (int i=0; i<this->count(); i++) {
        QListWidgetItem *item = this->item(i);
        item->setHidden(filter != ALL_TYPE && !item->text().toUpper().endsWith(filter));
    }
}

void FileListViewer::updateViewFromFileList(QVector<BsaFile> fileList)
{
    QStringList extensions;
    this->clear();
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
        this->addItem(file.fileName());
    }
    extensions.sort();
    extensions.prepend(ALL_TYPE);
    mFileExtensionFilter->addItems(extensions);
}
