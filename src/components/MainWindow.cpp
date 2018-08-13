#include "MainWindow.h"
#include "ToolBar.h"
#include "ConsoleDock.h"
#include "../log/Logger.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mBsaArchive()
{
    // Menu bar
    mMenuBar = new MenuBar();
    setMenuBar(mMenuBar);

    // Toolbar
    ToolBar *toolBar = new ToolBar(mMenuBar);
    addToolBar(toolBar);

    // Connecting MenuBar/ToolBar actions
    connect(mMenuBar->getNewBSAFileAction(), SIGNAL(triggered()),this, SLOT(newBsa()));
    connect(mMenuBar->getOpenBSAFileAction(), SIGNAL(triggered()),this, SLOT(openBsa()));
    connect(mMenuBar->getSaveBSAFileAction(), SIGNAL(triggered()),this, SLOT(saveBsa()));
    connect(mMenuBar->getCloseBSAFileAction(), SIGNAL(triggered()),this, SLOT(closeBsa()));

    // Connecting actions update slots
    connect(&mBsaArchive, SIGNAL(archiveOpened(bool)), mMenuBar, SLOT(updateActionsFromBsaArchiveState(bool)));
    connect(&mBsaArchive, SIGNAL(archiveClosed(bool)), mMenuBar, SLOT(updateActionsFromBsaArchiveState(bool)));
    mMenuBar->updateActionsFromBsaArchiveState(mBsaArchive.isOpened());

    // Central widget
    QWidget *mainZone = new QWidget;
    setCentralWidget(mainZone);
    QGridLayout *mainGrid = new QGridLayout;
    mainZone->setLayout(mainGrid);

    // Console
    QDockWidget *consoleDock = new ConsoleDock(this);
    this->addDockWidget(Qt::BottomDockWidgetArea, consoleDock);
    mMenuBar->getViewMenu()->addAction(consoleDock->toggleViewAction());
}

void MainWindow::askUserToConfirmClosingOfOpenedBsa()
{
    if (mBsaArchive.isOpened())
    {
        int ret = QMessageBox::question(this, "Open a BSA",
                                        "A BSA archive is already opened.\n"
                                        "If you open a new one, unsaved changes of the previous BSA will be discarded.\n"
                                        "Do you really want to open an other BSA archive?");
        if (ret == QMessageBox::Yes)
        {
            Status status = mBsaArchive.closeArchive();
            Logger::getInstance().logErrorOrInfo(status, QString("Archive closed."));
        }
    }
}

// Create a new empty bsa
void MainWindow::newBsa()
{
    this->askUserToConfirmClosingOfOpenedBsa();
    if (!mBsaArchive.isOpened())
    {
        Status status = mBsaArchive.createNewArchive();
        Logger::getInstance().logErrorOrInfo(status,
                                             QString("Archive created. %1 files, size: %2 bytes")
                                             .arg(mBsaArchive.getFileNumber())
                                             .arg(mBsaArchive.getModifiedSize()));
    }
}

// Open a bsa
void MainWindow::openBsa() {
    this->askUserToConfirmClosingOfOpenedBsa();
    if (!mBsaArchive.isOpened())
    {
        QString bsaDirectory(QDir::homePath());
        QString archiveFilePath = QFileDialog::getOpenFileName(this, "Open BSA archive",
                                                               bsaDirectory, "BSA archives (*.bsa *.BSA)");
        if (archiveFilePath != NULL)
        {
            Status status = mBsaArchive.openArchive(archiveFilePath);
            Logger::getInstance().logErrorOrInfo(status,
                                                 QString("Archive opened : %1")
                                                 .arg(archiveFilePath));
        }
    }
}

// Save a bsa
void MainWindow::saveBsa() {
    if (mBsaArchive.isOpened())
    {
        QString bsaDirectory(QDir::homePath());
        QString saveFilePath = QFileDialog::getSaveFileName(this, "Save BSA archive",
                                                               bsaDirectory, "BSA archives (*.bsa *.BSA)");
        if (saveFilePath != NULL)
        {
            Status status = mBsaArchive.saveArchive(saveFilePath);
            Logger::getInstance().logErrorOrInfo(status,
                                                 QString("Archive saved to %1")
                                                 .arg(saveFilePath));
        }
    }
}

// Close the opened bsa
void MainWindow::closeBsa()
{
    if (mBsaArchive.isOpened())
    {
        if (mBsaArchive.isModified())
        {
            int ret = QMessageBox::question(this, "Close BSA",
                                            "The opened BSA archive is modified.\n"
                                            "If you close it, changes will be lost.\n"
                                            "Do you really want to close this archive?");
            if (ret == QMessageBox::No)
            {
                return;
            }
        }
        Status status = mBsaArchive.closeArchive();
        Logger::getInstance().logErrorOrInfo(status, QString("Archive closed."));
    }
}
