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
FileListViewer::FileListViewer(QWidget *parent) : QWidget(parent) {
    // Building full widget
    auto *filterLayout = new QHBoxLayout;
    auto *filterLabel = new QLabel("File type");
    mFileExtensionFilter = new QComboBox;
    mFileExtensionFilter->addItem(ALL_TYPE);
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(mFileExtensionFilter);

    auto *mainVLayout = new QVBoxLayout(parent);
    mainVLayout->addLayout(filterLayout);
    mFileList = new QListWidget;
    mainVLayout->addWidget(mFileList);
    this->setLayout(mainVLayout);

    // Connecting
    connect(mFileExtensionFilter, SIGNAL(activated(QString)), SLOT(updateViewFromFilterChange(QString)));
    connect(mFileList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(emitCurrentItemChanged(QListWidgetItem *, QListWidgetItem *)));
}

//**************************************************************************
// Methods
//**************************************************************************
void selectFirstVisible(QListWidget *list) {
    for (int i = 0; i < list->count(); i++) {
        auto *item = (FileListViewerItem *) list->item(i);
        if (!item->isHidden()) {
            list->setCurrentItem(item);
            return;
        }
    }
}

void FileListViewer::updateViewFromFilterChange(const QString &filter) {
    for (int i = 0; i < mFileList->count(); i++) {
        auto *item = (FileListViewerItem *) mFileList->item(i);
        item->setHidden(filter != ALL_TYPE && !item->bsaFile().fileName()
                .toUpper().endsWith('.' + filter));
    }
    selectFirstVisible(mFileList);
}

void FileListViewer::updateViewFromFileList(const QVector<BsaFile> &fileList) {
    QString currentItemText = mFileList->currentItem() == nullptr ? "" : mFileList->currentItem()->text();
    QString currentFilter = mFileExtensionFilter->currentText();
    QStringList extensions;
    mFileList->clear();
    mFileExtensionFilter->clear();
    for (const auto &file : fileList) {
        int lastPointPosition = file.fileName().lastIndexOf('.');
        QString extension;
        if (lastPointPosition == -1) {
            extension = QString("No type");
        } else {
            extension = file.fileName().mid(lastPointPosition + 1).toUpper();
        }
        if (!extensions.contains(extension)) {
            extensions.append(extension);
        }
        auto *item = new FileListViewerItem;
        item->setBsaFile(file);
        item->setText(file.fileName());
        mFileList->addItem(item);
    }
    extensions.sort();
    extensions.prepend(ALL_TYPE);

    // TODO check if it is working when files can be modified
    mFileExtensionFilter->addItems(extensions);
    QList<QListWidgetItem *> itemList = mFileList->findItems(currentItemText, Qt::MatchExactly);
    // selecting previously selected item if exists, the first in the list otherwise
    if (extensions.contains(currentFilter) && itemList.size() == 1 && !itemList.at(0)->isHidden()) {
        mFileExtensionFilter->setCurrentText(currentFilter);
        mFileList->setCurrentItem(itemList.at(0));
    } else {
        selectFirstVisible(mFileList);
    }
}

void FileListViewer::emitCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    auto *currentFile = (FileListViewerItem *) current;
    auto *previousFile = (FileListViewerItem *) previous;
    emit currentFileItemChanged(currentFile != nullptr ? currentFile->bsaFile() : BsaFile::INVALID_BSAFILE,
                                previousFile != nullptr ? previousFile->bsaFile() : BsaFile::INVALID_BSAFILE);
}
