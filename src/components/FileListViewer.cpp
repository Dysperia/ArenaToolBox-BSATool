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

    mFileListView = new QListWidget;
    this->addWidget(mFileListView);

    // Connecting filter mecanic
    connect(mFileExtensionFilter, SIGNAL(activated(QString)), SLOT(updateFileListFromFilterSlot(QString)));

   // Passing throw signal from internal widget
    connect(mFileListView, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
}

void FileListViewer::updateViewFromFilterChange(QString filter) {

}

// Generate the file filter lists
void FileListViewer::updateViewFromFileList(QVector<BsaFile> fileList)
{
    QSet<QString> extensions;

    mFileListView->clear();
    for (int i(0); i < fileList.size(); i++)
    {
        BsaFile file = fileList.at(i);
        mFileListView->addItem(file.fileName());
    }

//        int lastPointPosition = file.fileName().lastIndexOf('.');
//            if (lastPointPosition == std::string::npos)
//            {
//                return "";
//            }
//            else
//            {
//                std::string ext = filename.substr(lastPointPosition+1);
//                std::string upperExt(ext);
//                std::transform(ext.begin(), ext.end(), upperExt.begin(), toupper);
//                return upperExt;
//            }


//        std::string ext = BSAFile::getInstance()->getFileExtension(i);
//        std::string upperExt(ext);
//        std::transform(ext.begin(), ext.end(), upperExt.begin(), toupper);
//        if (ext == "")
//        {
//            mapFilteredList[QString::fromStdString("NO EXT")].append(QString::fromStdString(name));
//        }
//        else
//        {
//            mapFilteredList[QString::fromStdString(upperExt)].append(QString::fromStdString(name));
//        }
//    }
//    for (std::map<QString, QStringList>::iterator it=mapFilteredList.begin(); it!=mapFilteredList.end(); it++)
//    {
//        FileFilterList.append(it->first);
//    }
//    mapFilteredList[QString::fromStdString("None")] = filelist;
//    FileFilterList.prepend(QString::fromStdString("None"));

//    // Sorting each file lists
//    int listNbr = FileFilterList.size();
//    for (int i(0); i<listNbr; i++)
//    {
//        QString filterName = FileFilterList[i];
//        QStringList *list = &mapFilteredList[filterName];
//        list->sort();
//        }
}
