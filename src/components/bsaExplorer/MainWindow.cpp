#include "MainWindow.h"
#include "components/toolbar/ToolBar.h"
#include "components/console/ConsoleDock.h"
#include "components/fileList/FileListViewer.h"
#include "log/Logger.h"

#include <QMessageBox>
#include <QFileDialog>

//******************************************************************************
// Constructors
//******************************************************************************
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Menu bar
    mMenuBar = new MenuBar();
    setMenuBar(mMenuBar);
    mArchiveConfigurationLoader.loadConfiguration(mApplicationConfiguration.getLastLoadedArchConf());
    mMenuBar->updateConfigurationActions(mArchiveConfigurationLoader.getConfigurationList(), mArchiveConfigurationLoader.getCurrent().getName());

    // Toolbar
    auto *toolBar = new ToolBar(mMenuBar);
    addToolBar(toolBar);

    // Central widget
    auto *mainZone = new QWidget;
    setCentralWidget(mainZone);
    auto *mainGrid = new QGridLayout;
    mainZone->setLayout(mainGrid);
    auto *fileListViewer = new FileListViewer;
    mainGrid->addWidget(fileListViewer, 0, 0);

    // File display
    mFileDisplayer = new FileDisplayer;
    mainGrid->addLayout(mFileDisplayer, 0, 1);
    mainGrid->setColumnStretch(1, 1);

    // Console
    QDockWidget *consoleDock = new ConsoleDock;
    this->addDockWidget(Qt::BottomDockWidgetArea, consoleDock);
    mMenuBar->getViewMenu()->addAction(consoleDock->toggleViewAction());

    // Charging archive configuration from user choice
    connect(mMenuBar, SIGNAL(configurationChange(QString)), &mArchiveConfigurationLoader, SLOT(loadConfiguration(QString)));
    connect(mMenuBar, SIGNAL(configurationChange(QString)), &mApplicationConfiguration, SLOT(setLastLoadedArchConf(QString)));

    // Connecting MenuBar actions
    connect(mMenuBar->getNewBSAFileAction(), SIGNAL(triggered()), SLOT(newBsa()));
    connect(mMenuBar->getOpenBSAFileAction(), SIGNAL(triggered()), SLOT(openBsa()));
    connect(mMenuBar->getSaveBSAFileAction(), SIGNAL(triggered()), SLOT(saveBsa()));
    connect(mMenuBar->getCloseBSAFileAction(), SIGNAL(triggered()), SLOT(closeBsa()));

    // Updating MenuBar actions state according to BSA state
    connect(&mBsaArchive, &BsaArchive::archiveOpened, mMenuBar, &MenuBar::updateActionsFromBsaArchiveState);
    connect(&mBsaArchive, &BsaArchive::archiveClosed, mMenuBar, &MenuBar::updateActionsFromBsaArchiveState);
    mMenuBar->updateActionsFromBsaArchiveState(mBsaArchive.isOpened());

    // Updating file list when BSA change
    connect(&mBsaArchive, &BsaArchive::fileListModified, fileListViewer, &FileListViewer::updateViewFromFileList);

    // Displaying selected file
    connect(fileListViewer, SIGNAL(currentFileItemChanged(BsaFile,BsaFile)), SLOT(updateOnFileSelected(BsaFile)));
}

//**************************************************************************
// Methods
//**************************************************************************
void MainWindow::askUserToConfirmClosingOfOpenedBsa() {
    if (mBsaArchive.isOpened()) {
        int ret = QMessageBox::question(this, "Open a BSA",
                                        "A BSA archive is already opened.\n"
                                        "If you open a new one, unsaved changes of the previous BSA will be discarded.\n"
                                        "Do you really want to open an other BSA archive?");
        if (ret == QMessageBox::Yes) {
            Status status = mBsaArchive.closeArchive();
            Logger::getInstance().logErrorOrInfo(status, QString("Archive closed."));
        }
    }
}

void MainWindow::newBsa() {
    this->askUserToConfirmClosingOfOpenedBsa();
    if (!mBsaArchive.isOpened()) {
        Status status = mBsaArchive.createNewArchive();
        Logger::getInstance().logErrorOrInfo(status,
                                             QString("Archive created. %1 files, size: %2 bytes")
                                                     .arg(mBsaArchive.getFileNumber())
                                                     .arg(mBsaArchive.getModifiedSize()));
    }
}

void MainWindow::openBsa() {
    this->askUserToConfirmClosingOfOpenedBsa();
    if (!mBsaArchive.isOpened()) {
        QString bsaDirectory(mApplicationConfiguration.getLastOpenedBsa());
        QString archiveFilePath = QFileDialog::getOpenFileName(this, "Open BSA archive",
                                                               bsaDirectory, "BSA archives (*.bsa *.BSA)");
        if (archiveFilePath != nullptr) {
            Status status = mBsaArchive.openArchive(archiveFilePath);
            Logger::getInstance().logErrorOrInfo(status,
                                                 QString("Archive opened : %1. %2 files, size: %3 bytes")
                                                         .arg(archiveFilePath)
                                                         .arg(mBsaArchive.getFileNumber())
                                                         .arg(mBsaArchive.getModifiedSize()));
            if (mBsaArchive.isOpened()) {
                mApplicationConfiguration.setLastOpenedBsa(archiveFilePath);
            }
        }
    }
}

void MainWindow::saveBsa() {
    if (mBsaArchive.isOpened()) {
        QString bsaDirectory(QDir::homePath());
        QString saveFilePath = QFileDialog::getSaveFileName(this, "Save BSA archive",
                                                            bsaDirectory, "BSA archives (*.bsa *.BSA)");
        if (saveFilePath != nullptr) {
            Status status = mBsaArchive.saveArchive(saveFilePath);
            Logger::getInstance().logErrorOrInfo(status,
                                                 QString("Archive saved to %1")
                                                         .arg(saveFilePath));
        }
    }
}

void MainWindow::closeBsa() {
    if (mBsaArchive.isOpened()) {
        if (mBsaArchive.isModified()) {
            int ret = QMessageBox::question(this, "Close BSA",
                                            "The opened BSA archive is modified.\n"
                                            "If you close it, changes will be lost.\n"
                                            "Do you really want to close this archive?");
            if (ret == QMessageBox::No) {
                return;
            }
        }
        Status status = mBsaArchive.closeArchive();
        Logger::getInstance().logErrorOrInfo(status, QString("Archive closed."));
    }
}

void MainWindow::updateOnFileSelected(const BsaFile& currentItem) {
    if (currentItem != BsaFile::INVALID_BSAFILE) {
        const QVector<char> fileData = mBsaArchive.getFileData(currentItem);
        mFileDisplayer->display(currentItem, fileData, mArchiveConfigurationLoader.getCurrent());
    } else {
        mFileDisplayer->display(currentItem, QVector<char>(), mArchiveConfigurationLoader.getCurrent());
    }
}
