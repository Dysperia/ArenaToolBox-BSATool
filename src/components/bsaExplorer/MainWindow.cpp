#include <components/bsaExplorer/MainWindow.h>
#include <components/toolbar/ToolBar.h>
#include <components/console/ConsoleDock.h>
#include <components/fileList/FileListViewer.h>
#include <log/Logger.h>
#include <QMessageBox>
#include <QFileDialog>
#include <components/fileDisplay/PaletteDisplayer.h>

//******************************************************************************
// Constructors
//******************************************************************************
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Menu bar
    mMenuBar = new MenuBar();
    setMenuBar(mMenuBar);
    mArchiveConfigurationLoader.loadConfiguration(mApplicationConfiguration.getLastLoadedArchConf());
    mMenuBar->updateConfigurationActions(mArchiveConfigurationLoader.getConfigurationList(),
                                         mArchiveConfigurationLoader.getCurrent().getName());

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
    connect(mMenuBar, &MenuBar::configurationChange, this,
            [&](const QString &confName) {
                Logger::getInstance().exception_handle_void(
                        [&]() { mArchiveConfigurationLoader.loadConfiguration(confName); },
                        [&] {
                            return QString("Loaded configuration : %1.").arg(confName);
                        });
            });
    connect(&mArchiveConfigurationLoader, &ArchiveConfigurationLoader::configurationLoaded, this,
            [&](const QString &confName) {
                Logger::getInstance().exception_handle_void(
                        [&]() { mApplicationConfiguration.setLastLoadedArchConf(confName); },
                        [&] {
                            return QString();
                        });
            });

    // Connecting MenuBar actions
    connect(mMenuBar->getNewBSAFileAction(), &QAction::triggered, this, &MainWindow::newBsa);
    connect(mMenuBar->getOpenBSAFileAction(), &QAction::triggered, this, &MainWindow::openBsa);
    connect(mMenuBar->getSaveBSAFileAction(), &QAction::triggered, this, &MainWindow::saveBsa);
    connect(mMenuBar->getCloseBSAFileAction(), &QAction::triggered, this, &MainWindow::closeBsa);
    connect(mMenuBar->getViewPaletteAction(), &QAction::triggered, this, &MainWindow::showPalette);

    // Updating MenuBar actions state according to BSA state
    connect(&mBsaArchive, &BsaArchive::archiveOpened, mMenuBar, &MenuBar::updateActionsFromBsaArchiveState);
    connect(&mBsaArchive, &BsaArchive::archiveClosed, mMenuBar, &MenuBar::updateActionsFromBsaArchiveState);
    mMenuBar->updateActionsFromBsaArchiveState(mBsaArchive.isOpened());

    // Updating file list when BSA change
    connect(&mBsaArchive, &BsaArchive::fileListModified, fileListViewer, &FileListViewer::updateViewFromFileList);

    // Displaying selected file
    connect(fileListViewer, &FileListViewer::FileListViewer::currentFileItemChanged, this,
            &MainWindow::updateOnFileSelected);
}

//**************************************************************************
// Methods
//**************************************************************************
void MainWindow::askUserToConfirmClosingOfModifiedBsaAndClose(const QString &title, const QString &content) {
    auto ret = mBsaArchive.isOpened() ? QMessageBox::Yes : QMessageBox::No;
    if (mBsaArchive.isModified()) {
        ret = QMessageBox::question(this, title, content);
    }
    if (ret == QMessageBox::Yes) {
        Logger::getInstance().exception_handle_void([&] {
            mBsaArchive.closeArchive();
        }, [&] {
            return QString("Archive closed.");
        });
    }
}

void MainWindow::newBsa() {
    this->askUserToConfirmClosingOfModifiedBsaAndClose(QString("Open a BSA"),
                                                       QString("A BSA archive is already opened.\n"
                                                               "If you open a new one, unsaved changes of the previous BSA will be discarded.\n"
                                                               "Do you really want to open an other BSA archive?"));
    if (!mBsaArchive.isOpened()) {
        Logger::getInstance().exception_handle_void([&] {
            mBsaArchive.createNewArchive();
        }, [&] {
            return QString("Archive created. %1 files, size: %2 bytes")
                    .arg(mBsaArchive.fileNumber())
                    .arg(mBsaArchive.size());
        });
    }
}

void MainWindow::openBsa() {
    this->askUserToConfirmClosingOfModifiedBsaAndClose(QString("Open a BSA"),
                                                       QString("A BSA archive is already opened.\n"
                                                               "If you open a new one, unsaved changes of the previous BSA will be discarded.\n"
                                                               "Do you really want to open an other BSA archive?"));
    if (!mBsaArchive.isOpened()) {
        QString bsaDirectory(mApplicationConfiguration.getLastOpenedBsa());
        QString archiveFilePath = QFileDialog::getOpenFileName(this, "Open BSA archive",
                                                               bsaDirectory, "BSA archives (*.bsa *.BSA)");
        if (!archiveFilePath.isEmpty()) {
            Logger::getInstance().exception_handle_void([&] {
                mBsaArchive.openArchive(archiveFilePath);
                mApplicationConfiguration.setLastOpenedBsa(archiveFilePath);
            }, [&] {
                return QString("Archive opened : %1. %2 files, size: %3 bytes")
                        .arg(archiveFilePath)
                        .arg(mBsaArchive.fileNumber())
                        .arg(mBsaArchive.size());
            });
        }
    }
}

void MainWindow::saveBsa() {
    if (mBsaArchive.isOpened()) {
        QString bsaDirectory(QDir::homePath());
        QString saveFilePath = QFileDialog::getSaveFileName(this, "Save BSA archive",
                                                            bsaDirectory, "BSA archives (*.bsa *.BSA)");
        if (!saveFilePath.isEmpty()) {
            Logger::getInstance().exception_handle_void([&] {
                mBsaArchive.saveArchive(saveFilePath);
            }, [&] {
                return QString("Archive saved to %1")
                        .arg(saveFilePath);
            });
        }
    }
}

void MainWindow::closeBsa() {
    this->askUserToConfirmClosingOfModifiedBsaAndClose(QString("Close BSA"),
                                                       QString("The opened BSA archive is modified.\n"
                                                               "If you close it, changes will be lost.\n"
                                                               "Do you really want to close this archive?"));
}

void MainWindow::updateOnFileSelected(const BsaFile &currentItem) {
    if (currentItem != BsaFile::INVALID_BSAFILE) {
        const QVector<char> fileData = mBsaArchive.getFileData(currentItem);
        mFileDisplayer->display(currentItem, fileData, mArchiveConfigurationLoader.getCurrent());
    } else {
        mFileDisplayer->display(currentItem, QVector<char>(), mArchiveConfigurationLoader.getCurrent());
    }
}

void MainWindow::showPalette() {
    auto *paletteDisplayer = new PaletteDisplayer(mFileDisplayer->getCurrentPalette().getColorTable());
    paletteDisplayer->setWindowTitle(
            mFileDisplayer->getCurrentFile().fileName() + " use palette : " + mFileDisplayer->getCurrentPaletteName());
    paletteDisplayer->show();
}
