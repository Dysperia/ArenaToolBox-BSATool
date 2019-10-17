#include "FileListViewer.h"
#include "FileListViewerItem.h"
#include "log/Logger.h"

#include <QLabel>

//******************************************************************************
// Statics
//******************************************************************************
const QString ALL_TYPE = QString("All types"); // NOLINT(cert-err58-cpp)

//******************************************************************************
// Constructors
//******************************************************************************
FileListViewer::FileListViewer(QWidget *parent)
{
    // Building full widget
    auto *filterLayout = new QHBoxLayout;
    QLabel *filterLabel = new QLabel("File type");
    mFileExtensionFilter = new QComboBox;
    mFileExtensionFilter->addItem(ALL_TYPE);
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
        auto *item = (FileListViewerItem*) this->item(i);
        item->setHidden(filter != ALL_TYPE && !item->bsaFile().fileName()
                                  .toUpper().endsWith('.' + filter));
    }
}

void FileListViewer::updateViewFromFileList(QVector<BsaFile> fileList)
{
    QString currentItemText = this->currentItem() == nullptr ? "" : this->currentItem()->text();
    QString currentFilter = mFileExtensionFilter->currentText();
    QStringList extensions;
    this->clear();
    mFileExtensionFilter->clear();
    for (const auto &file : fileList) {
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
        auto *item = new FileListViewerItem;
        item->setBsaFile(file);
        item->setText(file.fileName());
        this->addItem(item);
    }
    extensions.sort();
    extensions.prepend(ALL_TYPE);

    // TODO check if it is working when files can be modified
    mFileExtensionFilter->addItems(extensions);
    QList<QListWidgetItem*> itemList = this->findItems(currentItemText, Qt::MatchExactly);
    if (extensions.contains(currentFilter) && itemList.size() == 1) {
        mFileExtensionFilter->setCurrentText(currentFilter);
        this->setCurrentItem(itemList.at(0));
    }
}
