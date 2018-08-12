#include "MainWindow.h"
#include "ToolBar.h"
#include "ConsoleDock.h"
#include "../log/Logger.h"

#include <QGridLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mBsaArchive()
{
    // Menu bar
    mMenuBar = new MenuBar(&mBsaArchive);
    setMenuBar(mMenuBar);

    // Toolbar
    ToolBar *toolBar = new ToolBar(mMenuBar);
    addToolBar(toolBar);

    // Connecting MenuBar/ToolBar actions
    connect(mMenuBar->getNewBSAFileAction(), SIGNAL(triggered()),this, SLOT(newBsaSlot()));
    connect(mMenuBar->getCloseBSAFileAction(), SIGNAL(triggered()),this, SLOT(closeBsaSlot()));

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

// Create a new empty bsa
void MainWindow::newBsaSlot()
{
    if (mBsaArchive.isOpened())
    {
        int ret = QMessageBox::question(this, "Create a new BSA",
                                        "A BSA archive is already opened.\n"
                                        "If you create a new one, unsaved changes of the previous BSA will be discarded.\n"
                                        "Do you really want to create a new BSA archive?");
        if (ret == QMessageBox::Yes)
        {
            Status status = mBsaArchive.closeArchive();
            Logger::getInstance().logError(status);
        }
    }
    if (!mBsaArchive.isOpened())
    {
        Status status = mBsaArchive.createNewArchive();
        Logger::getInstance().logErrorOrInfo(status,
                                             QString("Archive created. %1 files, size: %2 bytes")
                                             .arg(mBsaArchive.getFileNumber())
                                             .arg(mBsaArchive.getModifiedSize()));
    }
}

// Close the opened bsa
void MainWindow::closeBsaSlot()
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
