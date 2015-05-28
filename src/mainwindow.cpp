#include "mainwindow.hpp"
#include "previewpalette.hpp"
#include "imageviewer.hpp"
#include "askuserwindow.hpp"
#include "various.hpp"
#include <string>
#include <QApplication>
#include <QWidget>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>
#include <QGridLayout>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>
#include <QAbstractButton>
#include <QPushButton>
#include <QCloseEvent>
#include <QPlainTextEdit>

// Constructor
MainWindow::MainWindow() : clearing(false), previewUpdate(false), isAnimationPlayed(false)
{
    // Setting up lastBSADirectory
    lastBSADirectory ="";
    lastOpenSaveFileDirectory ="";
    lastColDirectory = "";

    // Initializing image data and pixmap list to one frame
    imageData.resize(1);
    pixmapList.resize(1);

    std::ifstream prefStream;
    prefStream.open("data/settings");
    if (prefStream.is_open())
    {
        std::string lastBSADir = new char[300];
        std::string lastOpenSaveDir = new char[300];
        std::string lastColDir = new char[300];
        std::getline(prefStream, lastBSADir);
        std::getline(prefStream, lastOpenSaveDir);
        std::getline(prefStream, lastColDir);
        lastBSADirectory = QString::fromStdString(lastBSADir);
        lastOpenSaveFileDirectory = QString::fromStdString(lastOpenSaveDir);
        lastColDirectory = QString::fromStdString(lastColDir);
    }

    // Menu bar
    QMenuBar *menuBar = new QMenuBar;
    setMenuBar(menuBar);
    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *newBSAFile = new QAction("New BSA file", this);
    fileMenu->addAction(newBSAFile);
    newBSAFile->setIcon(QIcon("icon/new_bsa.png"));
    connect(newBSAFile, SIGNAL(triggered()),this, SLOT(newBsaSlot()));
    QAction *openBSAFile = new QAction("Open BSA file", this);
    fileMenu->addAction(openBSAFile);
    openBSAFile->setIcon(QIcon("icon/open_bsa.png"));
    connect(openBSAFile, SIGNAL(triggered()),this, SLOT(openArchiveSlot()));
    saveBSAFile = new QAction("Save BSA file", this);
    fileMenu->addAction(saveBSAFile);
    saveBSAFile->setIcon(QIcon("icon/save_bsa.png"));
    connect(saveBSAFile, SIGNAL(triggered()), this, SLOT(saveBSASlot()));
    addFile = new QAction("Add file", this);
    fileMenu->addAction(addFile);
    addFile->setIcon(QIcon("icon/add_file.png"));
    connect(addFile, SIGNAL(triggered()),this, SLOT(addFileSlot()));
    updateFile = new QAction("Update file(s)", this);
    fileMenu->addAction(updateFile);
    updateFile->setIcon(QIcon("icon/update_file.png"));
    connect(updateFile, SIGNAL(triggered()),this, SLOT(updateFileSlot()));
    deleteFile = new QAction("Delete file", this);
    fileMenu->addAction(deleteFile);
    deleteFile->setIcon(QIcon("icon/delete_file.png"));
    connect(deleteFile, SIGNAL(triggered()),this, SLOT(deleteFileSlot()));
    QMenu *cancelDeleteUpdateMenu = fileMenu->addMenu("Cancel delete/update file");
    cancelDeleteFile = new QAction("Cancel delete file", this);
    connect(cancelDeleteFile, SIGNAL(triggered()), this, SLOT(cancelDeleteFileSlot()));
    cancelUpdateFile = new QAction("Cancel update file", this);
    connect(cancelUpdateFile, SIGNAL(triggered()), this, SLOT(cancelUpdateFileSlot()));
    cancelDeleteUpdateMenu->addAction(cancelUpdateFile);
    cancelDeleteUpdateMenu->addAction(cancelDeleteFile);
    QAction *quitAction = new QAction("Quit", this);
    fileMenu->addAction(quitAction);
    quitAction->setIcon(QIcon("icon/exit.png"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    QMenu *viewMenu = menuBar->addMenu("View");
    extendedPreview = new QAction("Extended preview", this);
    viewMenu->addAction(extendedPreview);
    connect(extendedPreview, SIGNAL(triggered()), this, SLOT(viewExtendedPreviewSlot()));
    QAction *updatePreviewOverDefault = viewMenu->addAction("Use update file over default for image preview");
    updatePreviewOverDefault->setCheckable(true);
    connect(updatePreviewOverDefault, SIGNAL(toggled(bool)), this, SLOT(setUseUpdatePreviewSlot(bool)));

    QMenu *extractMenu = menuBar->addMenu("Extract");
    QMenu *extractAsRawMenu = extractMenu->addMenu("Extract as raw");
    extractRawFile = new QAction("Extract file", this);
    extractAsRawMenu->addAction(extractRawFile);
    connect(extractRawFile, SIGNAL(triggered()), this, SLOT(extractRawFileSlot()));
    extractRawAllFilteredFiles = new QAction("Extract all filtered files", this);
    extractAsRawMenu->addAction(extractRawAllFilteredFiles);
    connect(extractRawAllFilteredFiles, SIGNAL(triggered()), this, SLOT(extractRawAllFilteredFilesSlot()));
    QMenu *extractImageMenu = extractMenu->addMenu("Extract image");
    extractDecompressImage = new QAction("Extract and decompress IMG/SET", this);
    extractImageMenu->addAction(extractDecompressImage);
    connect(extractDecompressImage, SIGNAL(triggered()), this, SLOT(extractDecompressImageSlot()));
    extractDecompressAllImage = new QAction("Extract and decompress all IMG/SET", this);
    extractImageMenu->addAction(extractDecompressAllImage);
    connect(extractDecompressAllImage, SIGNAL(triggered()), this, SLOT(extractDecompressAllImageSlot()));
    extractDecompressConvertImage = new QAction("Extract, decompress and convert IMG/SET to PNG", this);
    extractImageMenu->addAction(extractDecompressConvertImage);
    connect(extractDecompressConvertImage, SIGNAL(triggered()), this, SLOT(extractDecompressConvertImageSlot()));
    extractDecompressConvertAllImage = new QAction("Extract, decompress and convert all IMG/SET to PNG", this);
    extractImageMenu->addAction(extractDecompressConvertAllImage);
    connect(extractDecompressConvertAllImage, SIGNAL(triggered()), this, SLOT(extractDecompressConvertAllImageSlot()));
    QMenu *extractAnimationMenu = extractMenu->addMenu("Extract animation");
    extractConvertAnimation = new QAction("Extract and convert DFA/CFA/CIF to GIF", this);
    extractAnimationMenu->addAction(extractConvertAnimation);
    extractConvertAllAnimation = new QAction("Extract and convert all DFA/CFA/CIF to GIF", this);
    extractAnimationMenu->addAction(extractConvertAllAnimation);
    QMenu *extractSoundMenu = extractMenu->addMenu("Extract sound");
    extractConvertSound = new QAction("Extract and convert VOC/XFM/XMI to WAV", this);
    extractSoundMenu->addAction(extractConvertSound);
    extractConvertAllSound = new QAction("Extract and convert all VOC/XFM/XMI to WAV", this);
    extractSoundMenu->addAction(extractConvertAllSound);

    QMenu *paletteMenu = menuBar->addMenu("Palette");
    viewPalette = new QAction("View palette", this);
    paletteMenu->addAction(viewPalette);
    viewPalette->setIcon(QIcon("icon/view_palette.png"));
    connect(viewPalette, SIGNAL(triggered()), this, SLOT(showPreviewPaletteSlot()));

    QMenu *toolMenu = menuBar->addMenu("Tool");
    QAction *encryptDecryptInf = new QAction("Encrypt/Decrypt INF file", this);
    toolMenu->addAction(encryptDecryptInf);
    connect(encryptDecryptInf, SIGNAL(triggered()), this, SLOT(encryptDecryptINFSlot()));
    QAction *decompressExternalIMG = new QAction("Decompress external IMG(s)", this);
    toolMenu->addAction(decompressExternalIMG);
    connect(decompressExternalIMG, SIGNAL(triggered()), this, SLOT(decompressExternalIMGSlot()));
    QAction *decompressExternalIMGsToPNGs = new QAction("Decompress and convert external IMG(s)/SET(s) to PNG(s)", this);
    toolMenu->addAction(decompressExternalIMGsToPNGs);
    connect(decompressExternalIMGsToPNGs, SIGNAL(triggered()), this, SLOT(decompressConvertExternalIMGSETSlot()));
    QAction *convertPNGsToIMGsSETs = new QAction("Convert external PNG(s) to IMG/SET", this);
    toolMenu->addAction(convertPNGsToIMGsSETs);
    connect(convertPNGsToIMGsSETs, SIGNAL(triggered()), this, SLOT(convertPNGToIMGSETSlot()));
    QAction *viewExternalIMG = new QAction("View an external IMG/SET", this);
    toolMenu->addAction(viewExternalIMG);
    connect(viewExternalIMG, SIGNAL(triggered()), this, SLOT(viewExternalIMGSlot()));
    QAction *viewExternalCFADFACIF = new QAction("View an external CFA/DFA/CIF", this);
    toolMenu->addAction(viewExternalCFADFACIF);

    QMenu *aboutMenu = menuBar->addMenu("About");
    QAction *about = new QAction("About", this);
    aboutMenu->addAction(about);
    about->setIcon(QIcon("icon/about.png"));
    connect(about, SIGNAL(triggered()), this, SLOT(aboutSlot()));



    // Delete when implemented
    viewExternalCFADFACIF->setDisabled(true);



    // Toolbar
    QToolBar *toolBar = new QToolBar;
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setWindowTitle("Toolbar");
    toolBar->addAction(newBSAFile);
    toolBar->addAction(openBSAFile);
    toolBar->addAction(saveBSAFile);
    toolBar->addSeparator();
    toolBar->addAction(addFile);
    toolBar->addAction(deleteFile);
    toolBar->addAction(updateFile);
    QToolButton *cancelDeleteUpdateButton = new QToolButton;
    cancelDeleteUpdateButton->setPopupMode(QToolButton::InstantPopup);
    cancelDeleteUpdateButton->setIcon(QIcon("icon/cancel_deleteUpdate.png"));
    cancelDeleteUpdateButton->setMenu(cancelDeleteUpdateMenu);
    toolBar->addWidget(cancelDeleteUpdateButton);
    toolBar->addSeparator();
    toolBar->addAction(viewPalette);
    toolBar->addSeparator();
    QToolButton *rawToolButton = new QToolButton;
    rawToolButton->setPopupMode(QToolButton::InstantPopup);
    rawToolButton->setIcon(QIcon("icon/raw.png"));
    rawToolButton->setMenu(extractAsRawMenu);
    toolBar->addWidget(rawToolButton);
    QToolButton *imageToolButton = new QToolButton;
    imageToolButton->setPopupMode(QToolButton::InstantPopup);
    imageToolButton->setIcon(QIcon("icon/image.png"));
    imageToolButton->setMenu(extractImageMenu);
    toolBar->addWidget(imageToolButton);
    QToolButton *animationToolButton = new QToolButton;
    animationToolButton->setPopupMode(QToolButton::InstantPopup);
    animationToolButton->setIcon(QIcon("icon/animation.png"));
    animationToolButton->setMenu(extractAnimationMenu);
    toolBar->addWidget(animationToolButton);
    QToolButton *soundToolButton = new QToolButton;
    soundToolButton->setPopupMode(QToolButton::InstantPopup);
    soundToolButton->setIcon(QIcon("icon/sound.png"));
    soundToolButton->setMenu(extractSoundMenu);
    toolBar->addWidget(soundToolButton);
    QToolButton *toolToolButton = new QToolButton;
    toolToolButton->setPopupMode(QToolButton::InstantPopup);
    toolToolButton->setIcon(QIcon("icon/tool.png"));
    toolToolButton->setMenu(toolMenu);
    toolBar->addWidget(toolToolButton);
    toolBar->setIconSize(QSize(36, 36));
    addToolBar(toolBar);

    viewMenu->addAction(toolBar->toggleViewAction());

    // Central widget
    QWidget *mainZone = new QWidget;
    setCentralWidget(mainZone);
    QGridLayout *mainGrid = new QGridLayout;
    mainZone->setLayout(mainGrid);

    QVBoxLayout *fileListLayout = new QVBoxLayout;
    QHBoxLayout *filterLayout = new QHBoxLayout;
    QLabel *filterLabel = new QLabel("Filter");
    fileFilter = new QComboBox;
    connect(fileFilter, SIGNAL(activated(QString)), this, SLOT(updateFileListFromFilterSlot(QString)));
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(fileFilter);
    fileListLayout->addLayout(filterLayout);
    archiveFileList = new QListWidget;
    fileListLayout->addWidget(archiveFileList);
    connect(archiveFileList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(updateWindowSlot(QListWidgetItem*)));
    mainGrid->addLayout(fileListLayout,0,0);

    QVBoxLayout *labelLayout = new QVBoxLayout;
    previewLabel = new QLabel("No preview available");
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setFixedWidth(320);
    previewLabel->setFixedHeight(200);
    previewLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    previewLabel->setLineWidth(2);
    labelLayout->addWidget(previewLabel);
    infoLabel = new QLabel;
    infoLabel->setAlignment(Qt::AlignLeft);
    infoLabel->setFixedWidth(320);
    infoLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    infoLabel->setLineWidth(2);
    std::string str("");
    generateInfoText(str);
    infoLabel->setText(infoText);
    labelLayout->addWidget(infoLabel);
    mainGrid->addLayout(labelLayout,0,1);

    // Status bar
    statusBar();

    // Setting up some QActions activation state
    updateQActionsState();
}

// Subclassing the close event to make verification on the archive state before closing application
void MainWindow::closeEvent (QCloseEvent *event)
{
    if (BSAFile::getInstance()->getIsModified() == true && BSAFile::getInstance()->getIsSaved() == false)
    {
        int ret = QMessageBox::question(this, "Changes not saved", "The current archive changes have not been saved.\n"
                                        "Do you really want to close the application?");
        if (ret == QMessageBox::Yes)
        {
            // To be sure animation thread will end even if it can continue after the main one closed
            isAnimationPlayed = false;
            if(animationThread.joinable())
            {
                animationThread.join();
            }
            event->accept();
            saveSettings();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        isAnimationPlayed = false;
        if(animationThread.joinable())
        {
            animationThread.join();
        }
        event->accept();
        saveSettings();
    }
}

// Play an animation in preview label
void MainWindow::playAnimation()
{
    if (imageData.size() != 0)
    {
        isAnimationPlayed = true;
        int i(0);
        previewLabel->setPixmap(pixmapList[i]);
        int lastFrameNb = imageData.size() - 1;
        if (lastFrameNb >=1)
        {
            i = 1;
        }
        std::chrono::steady_clock::time_point timePoint1 = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point timePoint2;
        std::chrono::duration<int,std::milli> timeSpan;
        std::chrono::duration<int,std::milli> timeBetweenUpdate = std::chrono::duration<int,std::milli>(500);
        while (isAnimationPlayed)
        {
            timePoint2 = std::chrono::steady_clock::now();
            timeSpan = std::chrono::duration_cast<std::chrono::duration<int,std::milli>>(timePoint2 - timePoint1);
            // Every 500 milliseconds
            if (timeSpan >= timeBetweenUpdate)
            {
                previewLabel->setPixmap(pixmapList[i]);
                timePoint1 = std::chrono::steady_clock::now();
                if (i == lastFrameNb)
                {
                    i = 0;
                }
                else
                {
                    i++;
                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::duration<int,std::milli>(50));
            }
        }
    }
}

// Update the bool previewUpdated according to the user choice
void MainWindow::setUseUpdatePreviewSlot(bool useUpdate)
{
    previewUpdate = useUpdate;
}

// Clearing function
void MainWindow::clear()
{
    extractRawFile->setDisabled(true);
    extractRawAllFilteredFiles->setDisabled(true);
    saveBSAFile->setDisabled(true);
    addFile->setDisabled(true);
    updateFile->setDisabled(true);
    deleteFile->setDisabled(true);
    extractDecompressImage->setDisabled(true);
    extractDecompressAllImage->setDisabled(true);
    extractDecompressConvertImage->setDisabled(true);
    extractDecompressConvertAllImage->setDisabled(true);
    extractConvertAnimation->setDisabled(true);
    extractConvertAllAnimation->setDisabled(true);
    extractConvertSound->setDisabled(true);
    extractConvertAllSound->setDisabled(true);

    imageData.clear();
    imageData.resize(1);
    pixmapList.clear();
    pixmapList.resize(1);
    std::string str("");
    generateInfoText(str);
    filelist.clear();
    archiveFileList->clear();
    FileFilterList.clear();
    mapFilteredList.clear();
    fileFilter->clear();
    previewLabel->clear();
    previewLabel->setText(QString::fromStdString("No preview available\n\nfor this file"));
}

// Open archive file
void MainWindow::openArchiveSlot()
{
    bool reloading (false);
    if (BSAFile::getInstance()->getIsOpened())
    {
        int ret = QMessageBox::question(this, "Load a bsa", "A BSA archive already exists.\n"
                                        "If you load a new one, unsaved changes of the previous BSA will be discarded.\n"
                                        "Do you really want to open a new BSA archive?");
        if (ret == QMessageBox::Yes)
        {
            reloading = true;
        }
    }
    if (!BSAFile::getInstance()->getIsOpened() || reloading == true)
    {
        QString BSADirectory(QDir::homePath());
        if (lastBSADirectory != "")
        {
            BSADirectory = lastBSADirectory;
        }
        QString archiveFileName = QFileDialog::getOpenFileName(this, "Open File", BSADirectory, "BSA archives (*.bsa)");
        if (archiveFileName != NULL)
        {
            if (reloading == true)
            {
                clearing = true;
                clear();
                BSAFile::getInstance()->clear();
                clearing = false;
                reloading = false;
            }
            int ret = BSAFile::getInstance()->openArchive(archiveFileName.toStdString());
            switch (ret)
            {
            case 0:
            {
                generateFileLists();
                fileFilter->addItems(FileFilterList);
                UpdateShowedFileList(QString::fromStdString("None"));
                statusBar()->showMessage("BSA archive has been successfully loaded.");
                std::string BSAPath = BSAFile::getInstance()->getArchiveFilePath();
                int fileNamePos = BSAPath.find_last_of("/\\");
                lastBSADirectory = QString::fromStdString(BSAPath.substr(0, fileNamePos));
                break;
            }
            case 1:
            {
                QMessageBox::warning(this, "BSA loading error", "Unable to load the BSA archive.\nThe archive was either corrupted or not a Arena BSA archive");
                statusBar()->showMessage("Unable to load the BSA archive.");
                break;
            }
            case 2:
            {
                QMessageBox::warning(this, "BSA loading error", "Unable to load the BSA archive.\nAn error occured while opening the archive for reading.");
                statusBar()->showMessage("Unable to load the BSA archive.");
                break;
            }
            }
        }
    }
}

// Save settings to file
void MainWindow::saveSettings()
{
    std::ofstream prefStream;
    prefStream.open("data/settings", std::ios_base::out | std::ios_base::trunc);
    if (prefStream.is_open())
    {
        prefStream << lastBSADirectory.toStdString() << std::endl;
        prefStream << lastOpenSaveFileDirectory.toStdString() << std::endl;
        prefStream << lastColDirectory.toStdString() << std::endl;
    }
}

// Update window according to the current file extension and state (new, updated or delete file)
void MainWindow::updateWindowSlot(QListWidgetItem *item)
{
    isAnimationPlayed = false;
    if (animationThread.joinable())
    {
        animationThread.join();
    }
    updatePreview(item);
    updateInfoLabel(item);
    updateQActionsState(item);
}

// Update the info displayed when a file is selected
void MainWindow::updateInfoLabel(QListWidgetItem* item)
{
    if (!clearing)
    {
        QString qfileName = item->text();
        std::string cfileName = qfileName.toStdString();
        generateInfoText(cfileName);
        infoLabel->setText(infoText);
    }
}

// Update file list when a new filter is selected
void MainWindow::updateFileListFromFilterSlot(QString filter)
{
    UpdateShowedFileList(filter);
}

// Generate the file filter lists
void MainWindow::generateFileLists()
{
    uint16_t nbr = BSAFile::getInstance()->getFileNumber();
    if (nbr != 0)
    {
        std::string name;
        for (uint16_t i(0); i < nbr; i++)
        {
            name = BSAFile::getInstance()->getFileName(i);
            filelist.append(QString::fromStdString(name));
            std::string ext = BSAFile::getInstance()->getFileExtension(i);
            std::string upperExt(ext);
            std::transform(ext.begin(), ext.end(), upperExt.begin(), toupper);
            if (ext == "")
            {
                mapFilteredList[QString::fromStdString("NO EXT")].append(QString::fromStdString(name));
            }
            else
            {
                mapFilteredList[QString::fromStdString(upperExt)].append(QString::fromStdString(name));
            }
        }
        for (std::map<QString, QStringList>::iterator it=mapFilteredList.begin(); it!=mapFilteredList.end(); it++)
        {
            FileFilterList.append(it->first);
        }
        mapFilteredList[QString::fromStdString("None")] = filelist;
        FileFilterList.prepend(QString::fromStdString("None"));

        // Sorting each file lists
        int listNbr = FileFilterList.size();
        for (int i(0); i<listNbr; i++)
        {
            QString filterName = FileFilterList[i];
            QStringList *list = &mapFilteredList[filterName];
            list->sort();
        }
    }
}

// Update showed file list from filter
void MainWindow::UpdateShowedFileList(const QString &filter)
{
    clearing = true;
    archiveFileList->clear();
    clearing = false;
    QStringList list = mapFilteredList[filter];
    for (int i(0); i<list.size(); i++)
    {
        QString fileName = list[i];
        archiveFileList->addItem(fileName);
        QListWidgetItem *item = archiveFileList->item(i);
        int index = BSAFile::getInstance()->getIndex(fileName.toStdString());
        if (BSAFile::getInstance()->getIsFileDelete(index))
        {
            item->setBackgroundColor(QColor(255, 0, 0));
        }
        else if (BSAFile::getInstance()->getIsFileUpdated(index))
        {
            item->setBackgroundColor(QColor(0, 0, 255));
        }
        else if (BSAFile::getInstance()->getIsFileNew(index))
        {
            item->setBackgroundColor(QColor(0, 255, 0));
        }
    }
    archiveFileList->setCurrentRow(0);
}

// Generate the QString used in the info QLabel
void MainWindow::generateInfoText(const std::string &fileName)
{
    if (fileName == "")
    {
        infoText = "<table><tr><td><b>Bsa archive:</b><td><tr><td>Name:<td><tr><td>Header:<td><tr><td>Size:<td><tr><td><b>File:</b><td><tr><td>Name:<td><tr>"
                "<td>Index:<td><tr><td>Type:<td><tr><td>Flags:<td><tr><td>Image size:<td><tr><td>Palette:<td><tr><td>Compression:<td><tr><td>Size:<td><tr>"
                "<td>Offset:<td></table>";
    }
    else
    {
        int index = BSAFile::getInstance()->getIndex(fileName);
        QString tempstr;
        tempstr = "<table><tr><td><b>Bsa archive:</b><td><tr><td>Name:<td>";
        tempstr += QString::fromStdString(BSAFile::getInstance()->getArchiveFileName());
        tempstr += "<tr><td>Header:<td>";
        tempstr += QString::number(BSAFile::getInstance()->getFileNumber());
        tempstr += " files<tr><td>Size:<td>";
        tempstr += QString::number(BSAFile::getInstance()->getArchiveSize());
        tempstr += " bytes";
        if (BSAFile::getInstance()->getArchiveModifiedSize() != 0)
        {
            tempstr += " (" + QString::number(BSAFile::getInstance()->getArchiveModifiedSize()) + ")";
        }
        tempstr += "<tr><td><b>File:</b><td>";
        if (BSAFile::getInstance()->getIsFileNew(index))
        {
            tempstr += "<font color = #00FF00>new </font>";
        }
        if (BSAFile::getInstance()->getIsFileUpdated(index))
        {
            tempstr += "<font color = #0000FF>updated </font>";
        }
        if (BSAFile::getInstance()->getIsFileDelete(index))
        {
            tempstr += "<font color = #FF0000>deleted </font>";
        }
        tempstr += "<tr><td>Name:<td>";
        tempstr += QString::fromStdString(BSAFile::getInstance()->getFileName(index));
        tempstr += "<tr><td>Index:<td>";
        tempstr += QString::number(index+1);
        tempstr += "<tr><td>Type:<td>";
        std::string ext = BSAFile::getInstance()->getFileExtension(index);
        tempstr += QString::fromStdString(ext);
        tempstr += " (";
        tempstr += QString::fromStdString(BSAFile::getInstance()->getFileType(ext));
        // If file is a IMG or SET and preview image has been built and displayed
        if ((BSAFile::getInstance()->getFileExtension(index) == "IMG" && previewLabel->pixmap() != 0) ||
                (BSAFile::getInstance()->getFileExtension(index) == "SET" && previewLabel->pixmap() != 0))
        {
            uint16_t flags = image.getImageFlags();
            tempstr += ")<tr><td>Flags:<td>";
            if (flags != 0xFFFF)
            {
                tempstr += QString::fromStdString(std::to_string(flags & 0x00FF));
                tempstr += " ";
                tempstr += QString::fromStdString(std::to_string(flags >> 8));
            }
            else
            {
                tempstr += "none";
            }
            tempstr += "<tr><td>Image size:<td>";
            tempstr += QString::number(image.getImageWidth());
            tempstr += " x ";
            tempstr += QString::number(image.getImageHeight());
            tempstr += " pixels<tr><td>Palette:<td>";
            tempstr += QString::fromStdString(image.getPaletteName());
            tempstr += "<tr><td>Compression:<td>";
            if ((flags & 0x00FF) == 0x0004)
            {
                tempstr += "compression 04";
            }
            else if ((flags & 0x00FF) == 0x0008)
            {
                tempstr += "compression 08";
            }
            else if ((flags & 0x00FF) == 0x0000 || flags == 0xFFFF )
            {
                tempstr += "none";
            }
            else
            {
                tempstr += "unknown";
            }
        }
        else
        {
            tempstr += ")<tr><td>Flags:<td><tr><td>Size/duration:<td><tr><td>Palette:<td><tr><td>Compression:<td>";
        }
        tempstr += "<tr><td>Size:<td>";
        tempstr += QString::number(BSAFile::getInstance()->getFileSize(index));
        tempstr += " bytes";
        if (BSAFile::getInstance()->getIsFileDelete(index))
        {
            tempstr += " (0)";
        }
        else if (BSAFile::getInstance()->getIsFileUpdated(index))
        {
            tempstr += " (" + QString::number(BSAFile::getInstance()->getUpdatedFileSize(index)) + ")";
        }
        tempstr += "<tr><td>Offset:<td>";
        if (!BSAFile::getInstance()->getIsFileNew(index))
        {
            tempstr += QString::number(BSAFile::getInstance()->getFileOffset(index));
        }
        tempstr += "</table>";
        infoText = tempstr;
    }
}

// Update the state of QActions according to the current file extension and state (new, updated or delete file)
void MainWindow::updateQActionsState(QListWidgetItem *item)
{
    if (!clearing)
    {
        std::string extension = "";
        bool isDeleted(false);
        bool isUpdated(false);
        if (item != 0)
        {
            QString fileName = item->text();
            int index = BSAFile::getInstance()->getIndex(fileName.toStdString());
            extension = BSAFile::getInstance()->getFileExtension(index);
            isDeleted = BSAFile::getInstance()->getIsFileDelete(index);
            isUpdated = BSAFile::getInstance()->getIsFileUpdated(index);
        }

        extractDecompressImage->setDisabled(true);
        extractDecompressAllImage->setDisabled(true);
        extractDecompressConvertImage->setDisabled(true);
        extractDecompressConvertAllImage->setDisabled(true);
        extractConvertAnimation->setDisabled(true);
        extractConvertAllAnimation->setDisabled(true);
        extractConvertSound->setDisabled(true);
        extractConvertAllSound->setDisabled(true);
        if (isDeleted)
        {
            cancelDeleteFile->setDisabled(false);
        }
        else
        {
            cancelDeleteFile->setDisabled(true);
        }
        if (isUpdated)
        {
            cancelUpdateFile->setDisabled(false);
        }
        else
        {
            cancelUpdateFile->setDisabled(true);
        }
        if (previewLabel->pixmap() != 0 && !isAnimationPlayed)
        {
            viewPalette->setDisabled(false);
            extendedPreview->setDisabled(false);
        }
        else if (extension == "INF")
        {
            extendedPreview->setDisabled(false);
        }
        else
        {
            viewPalette->setDisabled(true);
            extendedPreview->setDisabled(true);
        }
        if (BSAFile::getInstance()->getIsOpened())
        {
            addFile->setDisabled(false);
        }
        else
        {
            addFile->setDisabled(true);
        }
        if (BSAFile::getInstance()->getIsOpened() == true && BSAFile::getInstance()->getFileNumber() != 0)
        {
            extractRawFile->setDisabled(false);
            extractRawAllFilteredFiles->setDisabled(false);
            updateFile->setDisabled(false);
            deleteFile->setDisabled(false);
        }
        else
        {
            extractRawFile->setDisabled(true);
            extractRawAllFilteredFiles->setDisabled(true);
            updateFile->setDisabled(true);
            deleteFile->setDisabled(true);
        }
        if (BSAFile::getInstance()->getIsModified() == true && BSAFile::getInstance()->getIsSaved() == false)
        {
            saveBSAFile->setDisabled(false);
        }
        else
        {
            saveBSAFile->setDisabled(true);
        }
        if (extension == "IMG" || extension == "SET")
        {
            extractDecompressImage->setDisabled(false);
            extractDecompressConvertImage->setDisabled(false);
        }
        if (extension == "DFA" || extension == "CFA" || extension == "CIF")
        {
    //        extractConvertAnimation->setDisabled(false);
        }
        if (extension == "VOC" || extension == "XFM" || extension == "XMI")
        {
    //        extractConvertSound->setDisabled(false);
        }
        int nbrOfFilters = FileFilterList.size();
        for ( int i(0); i < nbrOfFilters; i++)
        {
            if (FileFilterList[i] == "IMG" || FileFilterList[i] == "SET")
            {
                extractDecompressAllImage->setDisabled(false);
                extractDecompressConvertAllImage->setDisabled(false);
            }
            if (FileFilterList[i] == "DFA" || FileFilterList[i] == "CFA" || FileFilterList[i] == "CIF")
            {
    //            extractConvertAllAnimation->setDisabled(false);
            }
            if (FileFilterList[i] == "VOC" || FileFilterList[i] == "XFM" || FileFilterList[i] == "XMI")
            {
    //            extractConvertAllSound->setDisabled(false);
            }
        }
    }
}

// Extract file
void MainWindow::extractRawFileSlot()
{
    QListWidgetItem *item = archiveFileList->currentItem();
    QString qfileName = item->text();
    std::string cfileName = qfileName.toStdString();
    int index = BSAFile::getInstance()->getIndex(cfileName);
    // cannot extract new file from archive
    if (BSAFile::getInstance()->getIsFileNew(index))
    {
        QMessageBox::information(this, "Extraction of file as raw", "The file cannot be extracted because it is marked as new.");
    }
    else
    {
        QString filePath = QFileDialog::getSaveFileName( this, "Choose save directory", lastOpenSaveFileDirectory + "/" + qfileName);
        if (filePath != NULL)
        {
            std::string cFilePath = filePath.toStdString();
            int fileNamePos = cFilePath.find_last_of("/\\");
            lastOpenSaveFileDirectory = QString::fromStdString(cFilePath.substr(0, fileNamePos));
            int ret = BSAFile::getInstance()->extractFile(filePath.toStdString(), index);
            switch (ret)
            {
            case 0:
            {
                statusBar()->showMessage("File successfully extracted.");
                break;
            }
            case 1:
            {
                QMessageBox::warning(this, "Extraction of file as raw", "The file could not be extracted.");
                statusBar()->showMessage("Error while extracting file.");
                break;
            }
            }
        }
    }
}

// Extract all file visible in filtered list
void MainWindow::extractRawAllFilteredFilesSlot()
{
    int success(0);
    QStringList fileList = mapFilteredList[fileFilter->currentText()];
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose extraction directory", lastOpenSaveFileDirectory, QFileDialog::ShowDirsOnly);
    if (dirPath != NULL)
    {
        lastOpenSaveFileDirectory = dirPath;
        statusBar()->clearMessage();
        for (int i(0); i < fileList.size(); i++)
        {
            QString qfileName = fileList[i];
            std::string cfileName = qfileName.toStdString();
            int index = BSAFile::getInstance()->getIndex(cfileName);
            // Check to not try to extract a new file
            if (!BSAFile::getInstance()->getIsFileNew(index))
            {
                QString filePath = dirPath + "/" + qfileName;
                int ret = BSAFile::getInstance()->extractFile(filePath.toStdString(), index);
                if (ret > success)
                {
                    success = ret;
                }
            }
        }
        if (success == 0)
        {
            statusBar()->showMessage("File(s) successfully extracted.");
        }
        else
        {
            QMessageBox::warning(this, "Extract all visible as raw", "Some or all files could not be extracted.");
            statusBar()->showMessage("Error while extracting some file(s).");
        }
    }
}

// Build preview
void MainWindow::updatePreview(QListWidgetItem *item)
{
    if (!clearing)
    {
        QString qfileName = item->text();
        int index = BSAFile::getInstance()->getIndex(qfileName.toStdString());
        previewLabel->setText(QString::fromStdString("No preview available\nfor this file"));
        previewLabel->setAlignment(Qt::AlignCenter);
        std::string extension = BSAFile::getInstance()->getFileExtension(index);
        if (extension == "IMG" || extension == "SET")
        {
            if (BSAFile::getInstance()->getIsFileUpdated(index) && previewUpdate)
            {
                std::string filePath = BSAFile::getInstance()->getUpdatedFilePath(index);
                imageData[0].imageQImage = image.buildQImage(filePath);
                imageData[0].hasTransparency = image.getTransparency();
            }
            else if (BSAFile::getInstance()->getIsFileNew(index))
            {
                std::string filePath = BSAFile::getInstance()->getNewFilePath(index);
                imageData[0].imageQImage = image.buildQImage(filePath);
                imageData[0].hasTransparency = image.getTransparency();
            }
            else
            {
                imageData[0].imageQImage = image.buildQImage(index, extension);
                imageData[0].hasTransparency = image.getTransparency();
            }
            pixmapList[0].convertFromImage(imageData[0].imageQImage);
            if (pixmapList[0].size() != QSize(0,0))
            {
                if (pixmapList[0].height() > 200 || pixmapList[0].width() > 320)
                {
                    if ((pixmapList[0].height() - 200) < (pixmapList[0].width() - 320))
                    {
                        pixmapList[0] = pixmapList[0].scaledToWidth(320);
                    }
                    else
                    {
                        pixmapList[0] = pixmapList[0].scaledToHeight(200);
                    }
                }
                previewLabel->setPixmap(pixmapList[0]);
                previewFileName = qfileName;
                imageData[0].imagePaletteName = image.getPaletteName();
                imageData[0].imagePalette = image.getPalette(imageData[0].imagePaletteName, image.getTransparency());
            }
            else
            {
                previewLabel->setText("No preview available for this file\n"
                                      "because format was unknown");
                previewLabel->setAlignment(Qt::AlignCenter);
            }
        }
        else if (extension == "CIF")
        {
            if (BSAFile::getInstance()->getIsFileUpdated(index) && previewUpdate)
            {
                std::string filePath = BSAFile::getInstance()->getUpdatedFilePath(index);
                imageData = image.buildAnimation(index, filePath);
                pixmapList.resize(imageData.size());
                for (size_t i(0); i<imageData.size(); i++)
                {
                    pixmapList[i].convertFromImage(imageData[i].imageQImage);
                }
            }
            else if (BSAFile::getInstance()->getIsFileNew(index))
            {
                std::string filePath = BSAFile::getInstance()->getUpdatedFilePath(index);
                imageData = image.buildAnimation(index, filePath);
                pixmapList.resize(imageData.size());
                for (size_t i(0); i<imageData.size(); i++)
                {
                    pixmapList[i].convertFromImage(imageData[i].imageQImage);
                }
            }
            else
            {
                imageData = image.buildAnimation(index);
                pixmapList.resize(imageData.size());
                for (size_t i(0); i<imageData.size(); i++)
                {
                    pixmapList[i].convertFromImage(imageData[i].imageQImage);
                }
            }
            if (pixmapList.size() != 0)
            {
                // If the previous thread has not been ended (it should have),
                // destroyed will be called when using "=", so safe here to reassign a new one
                animationThread = std::thread(&MainWindow::playAnimation, this);
            }
            else
            {
                imageData.clear();
                imageData.resize(1);
                pixmapList.clear();
                pixmapList.resize(1);
            }
        }
        else if (extension == "INF")
        {
            std::ifstream *stream = BSAFile::getInstance()->getStream();
            size_t fileOffset = BSAFile::getInstance()->getFileOffset(index);
            size_t fileSize = BSAFile::getInstance()->getFileSize(index);
            stream->seekg(fileOffset, std::ios_base::beg);
            char *INFData = new char[fileSize + 1];
            stream->read(INFData, fileSize);
            Compression::encryptDecryptINF(INFData, fileSize);
            // INF file data does not finish on a 0x00. Have to end the string manually
            INFData[fileSize] = '\0';
            INFContent = QString(INFData);
            previewLabel->setText(INFContent);
            previewLabel->setAlignment(Qt::AlignLeft);
            delete[] INFData;
        }
    }
}

// Show some application infos
void MainWindow::aboutSlot()
{
   QMessageBox::information(this, "About", "<html>This program has been designed to create and edit Bethesda Arena BSA file<br><br>"
                            "Application created by Dysperia under Qt 5 with QtCreator 3<br><br>"
                            "If you find any bug, please, send an french/english email to<br>"
                            "<font color = #9C00FF>softwatermermaid@hotmail.fr</font>");
   this->test();
}

// Show the palette used by the current preview image
void MainWindow::showPreviewPaletteSlot()
{
    PreviewPalette *previewPaletteWindow = new PreviewPalette(imageData[0].imagePalette);
    if (imageData[0].imagePaletteName == "Integrated")
    {
        previewPaletteWindow->setWindowTitle(QString::fromStdString(imageData[0].imagePaletteName) + " " + previewFileName);
    }
    else
    {
        previewPaletteWindow->setWindowTitle(QString::fromStdString(imageData[0].imagePaletteName));
    }
    previewPaletteWindow->show();
}

// Extract and decompress IMG/SET
void MainWindow::extractDecompressImageSlot()
{
    QListWidgetItem *item = archiveFileList->currentItem();
    QString fileName = item->text();
    QString filePath = QFileDialog::getSaveFileName( this, "Choose save directory", lastOpenSaveFileDirectory + "/" + fileName);
    if (filePath != NULL)
    {
        std::string cfilePath = filePath.toStdString();
        int fileNamePos = cfilePath.find_last_of("/\\");
        lastOpenSaveFileDirectory = QString::fromStdString(cfilePath.substr(0, fileNamePos));
        int index = BSAFile::getInstance()->getIndex(fileName.toStdString());
        if (BSAFile::getInstance()->getIsFileNew(index) == false)
        {
            int ret = image.extractDecompressImage(index, cfilePath);
            switch (ret)
            {
            case 0:
            {
                statusBar()->showMessage("File successfully decompressed and extracted.");
                break;
            }
            case 1:
            {
                QMessageBox::warning(this, "Extract and decompress IMG/SET", "Error while extracting the file.");
                statusBar()->showMessage("Error while extracting file.");
                break;
            }
            case 2:
            {
                statusBar()->showMessage("IMG file had a unknown header, extracted as raw.");
                break;
            }
            }
        }
    }
}

// Extract and decompress all IMG/SET
void MainWindow::extractDecompressAllImageSlot()
{
    int success(0), error(0), extractedAsRaw(0);
    // Success IMG: 0: ok, 1: error(s) occured, 2: some extracted as raw but no error, 3: error and some extracted as raw, 4: subdir not created
    // Success SET: 0: ok, 1: error(s) occured, 2: subdir not created
    int IMGSuccess(0), SETSuccess(0);
    bool atLeastOneIMG(false), atLeastOneSET(false);
    int nbrOfFilters = FileFilterList.size();
    for ( int i(0); i < nbrOfFilters; i++)
    {
        if (FileFilterList[i] == "IMG")
        {
            atLeastOneIMG = true;
        }
        if (FileFilterList[i] == "SET")
        {
            atLeastOneSET = true;
        }
    }
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose IMG/SET extraction directory", lastOpenSaveFileDirectory, QFileDialog::ShowDirsOnly);
    if (dirPath != NULL)
    {
        lastOpenSaveFileDirectory = dirPath;
        QDir QdirPath(dirPath);
        if (atLeastOneIMG)
        {
            if (QdirPath.mkpath(dirPath + "/IMG"))
            {
                success = 0;
                error = 0;
                extractedAsRaw =0;
                QStringList IMGFileList = mapFilteredList["IMG"];
                int IMGNbr = IMGFileList.size();
                for (int i(0); i<IMGNbr; i++)
                {
                    QString fileName = IMGFileList[i];
                    QString filePath = dirPath + "/" + "IMG" + "/" + fileName;
                    int index = BSAFile::getInstance()->getIndex(fileName.toStdString());
                    if (BSAFile::getInstance()->getIsFileNew(index) == false)
                    {
                        int ret = image.extractDecompressImage(index, filePath.toStdString());
                        switch (ret)
                        {
                        case 0:
                        {
                            if (error == 0)
                            {
                                success = 1;
                            }
                            break;
                        }
                        case 1:
                        {
                            if (success == 1)
                            {
                                success = 0;
                            }
                            error = 1;
                            break;
                        }
                        case 2:
                        {
                            extractedAsRaw = 1;
                            break;
                        }
                        }
                    }
                }
                if (success == 1)
                {
                    if (extractedAsRaw == 0)
                    {
                        IMGSuccess = 0;
                    }
                    else
                    {
                        IMGSuccess = 2;
                    }
                }
                else
                {
                    if (extractedAsRaw == 0)
                    {
                        IMGSuccess = 1;
                    }
                    else
                    {
                        IMGSuccess = 3;
                    }
                }
            }
            else
            {
                IMGSuccess = 4;
            }
        }
        if (atLeastOneSET)
        {
            if (QdirPath.mkpath(dirPath + "/SET"))
            {
                success = 0;
                error = 0;
                QStringList SETFileList = mapFilteredList["SET"];
                int SETNbr = SETFileList.size();
                for (int i(0); i<SETNbr; i++)
                {
                    QString fileName = SETFileList[i];
                    QString filePath = dirPath + "/" + "SET" + "/" + fileName;
                    int index = BSAFile::getInstance()->getIndex(fileName.toStdString());
                    if (BSAFile::getInstance()->getIsFileNew(index) == false)
                    {
                        int ret = image.extractDecompressImage(index, filePath.toStdString());
                        switch (ret)
                        {
                        case 0:
                        {
                            if (error == 0)
                            {
                                success = 1;
                            }
                            break;
                        }
                        case 1:
                        {
                            if (success == 1)
                            {
                                success = 0;
                            }
                            error = 1;
                            break;
                        }
                        }
                    }
                }
                if (success == 1)
                {
                    IMGSuccess = 0;
                }
                else
                {
                    IMGSuccess = 1;
                }
            }
            else
            {
                SETSuccess = 2;
            }
        }
        QString message("");
        switch (IMGSuccess)
        {
        case 0:
        {
            message += "All IMG files where decompressed and extracted successfully.";
            break;
        }
        case 1:
        {
            message += "Some or all IMG could not be extrated for an unknown reason.";
            break;
        }
        case 2:
        {
            message += "Some IMG with unknown header where extracted as raw, all other IMG files where decompressed and extracted successfully.";
            break;
        }
        case 3:
        {
            message += "Some IMG could not be extrated for an unknown reason and some other with unknown header where extracted as raw.";
            break;
        }
        case 4:
        {
            message += "Subdirectory /IMG cound not be created. No IMG extracted.";
            break;
        }
        }
        message += "\n\n";
        switch (SETSuccess)
        {
        case 0:
        {
            message += "All SET files where decompressed and extracted successfully.";
            break;
        }
        case 1:
        {
            message += "Some or all SET could not be extrated for an unknown reason.";
            break;
        }
        case 2:
        {
            message += "Subdirectory /SET cound not be created. No SET extracted.";
            break;
        }
        }
        if (IMGSuccess > 1 || SETSuccess > 1)
        {
            QMessageBox::warning(this, "Extraction of all IMG/SET", message);
            statusBar()->showMessage("Some error(s) or warning(s) during decompression and extraction of all IMG/SET");
        }
        else
        {
            QMessageBox::information(this, "Extraction of all IMG/SET", message);
            statusBar()->showMessage("All IMG/SET files successfully decompressed and extracted");
        }
    }
}

// Extract, decompress and convert IMG/SET
void MainWindow::extractDecompressConvertImageSlot()
{
    QListWidgetItem *item = archiveFileList->currentItem();
    std::string fileName = item->text().toStdString();
    int pos = fileName.find_last_of(".");
    std::string saveFileName(fileName);
    saveFileName.replace(pos, fileName.size()-pos, ".PNG");
    QString filePath = QFileDialog::getSaveFileName( this, "Choose save directory", lastOpenSaveFileDirectory + "/" + QString::fromStdString(saveFileName), "png image (*.png)");
    if (filePath != NULL)
    {
        std::string cfilePath = filePath.toStdString();
        int fileNamePos = cfilePath.find_last_of("/\\");
        lastOpenSaveFileDirectory = QString::fromStdString(cfilePath.substr(0, fileNamePos));
        int index = BSAFile::getInstance()->getIndex(fileName);
        if (BSAFile::getInstance()->getIsFileNew(index) == false)
        {
            int ret = image.extractDecompressConvertImage(index, cfilePath);
            switch (ret)
            {
            case 0:
            {
                statusBar()->showMessage("File successfully decompressed, converted and extracted.");
                break;
            }
            case 1:
            {
                QMessageBox::warning(this, "Extract, decompress and convert IMG/SET", "Error while extracting the file.");
                statusBar()->showMessage("Error while extracting, decompressing and converting file.");
                break;
            }
            case 2:
            {
                QMessageBox::information(this, "Extract, decompress and convert IMG/SET", "IMG file had a unknown header and was not extracted");
                statusBar()->showMessage("IMG file had a unknown header, not extracted.");
                break;
            }
            }
        }
    }
}

// Extract, decompress and convert All IMG/SET
void MainWindow::extractDecompressConvertAllImageSlot()
{
    int success(0), error(0), raw(0);
    // Success IMG: 0: ok, 1: error(s) occured, 2: some not extracted but no error, 3: error(s) and some not extracted, 4: subdir not created
    // Success SET: 0: ok, 1: error(s) occured, 2: subdir not created
    int IMGSuccess(0), SETSuccess(0);
    bool atLeastOneIMG(false), atLeastOneSET(false);
    int nbrOfFilters = FileFilterList.size();
    for ( int i(0); i < nbrOfFilters; i++)
    {
        if (FileFilterList[i] == "IMG")
        {
            atLeastOneIMG = true;
        }
        if (FileFilterList[i] == "SET")
        {
            atLeastOneSET = true;
        }
    }
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose IMG/SET extraction directory", lastOpenSaveFileDirectory, QFileDialog::ShowDirsOnly);
    if (dirPath != NULL)
    {
        lastOpenSaveFileDirectory = dirPath;
        QDir QdirPath(dirPath);
        if (atLeastOneIMG)
        {
            if (QdirPath.mkpath(dirPath + "/IMG"))
            {
                success = 0;
                error = 0;
                raw =0;
                QStringList IMGFileList = mapFilteredList["IMG"];
                int IMGNbr = IMGFileList.size();
                for (int i(0); i<IMGNbr; i++)
                {
                    std::string fileName = IMGFileList[i].toStdString();
                    int index = BSAFile::getInstance()->getIndex(fileName);
                    if (BSAFile::getInstance()->getIsFileNew(index) == false)
                    {
                        int pos = fileName.find_last_of(".");
                        std::string saveFileName(fileName);
                        saveFileName.replace(pos, fileName.size()-pos, ".PNG");
                        QString filePath = dirPath + "/" + "IMG" + "/" + QString::fromStdString(saveFileName);
                        int ret = image.extractDecompressConvertImage(index, filePath.toStdString());
                        switch (ret)
                        {
                        case 0:
                        {
                            if (error == 0)
                            {
                                success = 1;
                            }
                            break;
                        }
                        case 1:
                        {
                            if (success == 1)
                            {
                                success = 0;
                            }
                            error = 1;
                            break;
                        }
                        case 2:
                        {
                            raw = 1;
                            break;
                        }
                        }
                    }
                }
                if (success == 1)
                {
                    if (raw == 0)
                    {
                        IMGSuccess = 0;
                    }
                    else
                    {
                        IMGSuccess = 2;
                    }
                }
                else
                {
                    if (raw == 0)
                    {
                        IMGSuccess = 1;
                    }
                    else
                    {
                        IMGSuccess = 3;
                    }
                }
            }
            else
            {
                IMGSuccess = 4;
            }
        }
        if (atLeastOneSET)
        {
            if (QdirPath.mkpath(dirPath + "/SET"))
            {
                success = 0;
                error = 0;
                QStringList SETFileList = mapFilteredList["SET"];
                int SETNbr = SETFileList.size();
                for (int i(0); i<SETNbr; i++)
                {
                    std::string fileName = SETFileList[i].toStdString();
                    int index = BSAFile::getInstance()->getIndex(fileName);
                    if (BSAFile::getInstance()->getIsFileNew(index) == false)
                    {
                        int pos = fileName.find_last_of(".");
                        std::string saveFileName(fileName);
                        saveFileName.replace(pos, fileName.size()-pos, ".PNG");
                        QString filePath = dirPath + "/" + "SET" + "/" + QString::fromStdString(saveFileName);
                        int ret = image.extractDecompressConvertImage(index, filePath.toStdString());
                        switch (ret)
                        {
                        case 0:
                        {
                            if (error == 0)
                            {
                                success = 1;
                            }
                            break;
                        }
                        case 1:
                        {
                            if (success == 1)
                            {
                                success = 0;
                            }
                            error = 1;
                            break;
                        }
                        }
                    }
                }
                if (success == 1)
                {
                    IMGSuccess = 0;
                }
                else
                {
                    IMGSuccess = 1;
                }
            }
            else
            {
                SETSuccess = 2;
            }
        }
        QString message("");
        switch (IMGSuccess)
        {
        case 0:
        {
            message += "All IMG files where decompressed, converted and extracted successfully.";
            break;
        }
        case 1:
        {
            message += "Some or all IMG could not be extrated for an unknown reason.";
            break;
        }
        case 2:
        {
            message += "Some IMG with unknown header where not extracted, all other IMG files where decompressed, converted and extracted successfully.";
            break;
        }
        case 3:
        {
            message += "Some IMG could not be extrated for an unknown reason and some other IMG with unknown header where not extracted either.";
            break;
        }
        case 4:
        {
            message += "Subdirectory /IMG cound not be created. No IMG extracted.";
            break;
        }
        }
        message += "\n\n";
        switch (SETSuccess)
        {
        case 0:
        {
            message += "All SET files where decompressed, converted and extracted successfully.";
            break;
        }
        case 1:
        {
            message += "Some or all SET could not be extrated for an unknown reason.";
            break;
        }
        case 2:
        {
            message += "Subdirectory /SET cound not be created. No SET extracted.";
            break;
        }
        }
        if (IMGSuccess > 1 || SETSuccess > 1)
        {
            QMessageBox::warning(this, "Extraction of all IMG/SET", message);
            statusBar()->showMessage("Some error(s) or warning(s) during decompression, conversion and extraction of all IMG/SET");
        }
        else
        {
            QMessageBox::information(this, "Extraction of all IMG/SET", message);
            statusBar()->showMessage("All IMG/SET files successfully decompressed, converted and extracted");
        }
    }
}

// Decompress external IMG(s)
void MainWindow::decompressExternalIMGSlot()
{
    bool success(true), error(false), noHeadOrNotComp(false), unknownHead(false), notIMG(false);
    QStringList sourceFilesPath = QFileDialog::getOpenFileNames(this, "Choose IMG file(s) to open", lastOpenSaveFileDirectory, "IMG files (*.img)");
    if (sourceFilesPath.size() != 0)
    {
        int nbrOfIMGs = sourceFilesPath.size();
        for (int i(0); i<nbrOfIMGs; i++)
        {
            std::string filePath = sourceFilesPath[i].toStdString();
            int fileNamePos = filePath.find_last_of("/\\");
            lastOpenSaveFileDirectory = QString::fromStdString(filePath.substr(0, fileNamePos));
            std::string fileName = filePath.substr(fileNamePos + 1);
            int ret = image.decompressExternalIMG(fileName, filePath);
            switch (ret)
            {
            case 0:
            {
                break;
            }
            case 1:
            {
                if (success)
                {
                    success = false;
                }
                error = true;
                break;
            }
            case 2:
            {
                noHeadOrNotComp = true;
                break;
            }
            case 3:
            {
                unknownHead = true;
                break;
            }
            case 4:
            {
                notIMG = true;
                break;
            }
            }
        }
        QString message("");
        if (!error)
        {
            message += "No error were reported.";
            if (noHeadOrNotComp)
            {
                message += "\n\nSome files with no header or not compressed were left unmodified.";
            }
            if (unknownHead)
            {
                message += "\n\nSome files with unknown compression flag were left unmodified.";
            }
            if (notIMG)
            {
                message += "\n\nSome files not IMG files were left unmodified.";
            }
            QMessageBox::information(this, "decompression of external IMG(s)", message);
        }
        else
        {
            message += "Some errors when opening/writting files were reported.";
            if (noHeadOrNotComp)
            {
                message += "\n\nSome files with no header or not compressed were left unmodified.";
            }
            if (unknownHead)
            {
                message += "\n\nSome files with unknown compression flag were left unmodified.";
            }
            if (notIMG)
            {
                message += "\n\nSome files not IMG files were left unmodified.";
            }
            QMessageBox::warning(this, "decompression of external IMG(s)", message);
        }
    }
}

// View an external IMG/SET file
void MainWindow::viewExternalIMGSlot()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Choose IMG/SET file to open", lastOpenSaveFileDirectory, "IMG/SET files (*.img *.set)");
    if (filePath != NULL)
    {
        ImageViewer *externalViewer = new ImageViewer(filePath, "External viewer");
        externalViewer->show();
    }
}

// View an extended preview window for the actual IMG/SET in preview
void MainWindow::viewExtendedPreviewSlot()
{
    QString fileName = archiveFileList->currentItem()->text();
    int index = BSAFile::getInstance()->getIndex(fileName.toStdString());
    std::string extension = BSAFile::getInstance()->getFileExtension(index);
    if ((extension == "IMG" && imageData[0].imageQImage.size() != QSize(0,0)) || (extension == "SET" && imageData[0].imageQImage.size() != QSize(0,0)))
    {
        ImageViewer *imageExtendedPreview;
        if (imageData[0].imagePaletteName == "Integrated")
        {
            imageExtendedPreview = new ImageViewer(imageData[0].imageQImage, previewFileName, "Extended preview", imageData[0].hasTransparency, imageData[0].imagePaletteName, imageData[0].imagePalette);
        }
        else
        {
            imageExtendedPreview = new ImageViewer(imageData[0].imageQImage, previewFileName, "Extended preview", imageData[0].hasTransparency, imageData[0].imagePaletteName);
        }
        imageExtendedPreview->show();
    }
    else if (extension == "INF")
    {
        QWidget *INFExtendedPreview = new QWidget;
        INFExtendedPreview->setWindowTitle("Extended preview : " + fileName);
        QGridLayout *previewLayout = new QGridLayout;
        INFExtendedPreview->setLayout(previewLayout);
        QPlainTextEdit *previewPlainTextEdit = new QPlainTextEdit(INFContent);
        previewPlainTextEdit->setReadOnly(true);
        previewLayout->addWidget(previewPlainTextEdit);
        INFExtendedPreview->show();
    }
}

// Decompress and convert external IMGs/SETs to PNG
void MainWindow::decompressConvertExternalIMGSETSlot()
{
    bool setAll(false);
    std::string choice("PAL.COL");
    QVector<QRgb> customPalette;
    QStringList sourceFilesPath = QFileDialog::getOpenFileNames(this, "Choose IMG/SET file(s) to open", lastOpenSaveFileDirectory, "IMG/SET files (*.img *.set)");
    int nbrOfIMGs = sourceFilesPath.size();
    for (int i(0); i<nbrOfIMGs; i++)
    {
        std::string filePath = sourceFilesPath[i].toStdString();
        QImage Img = image.buildQImage(filePath);
        if (Img.size() != QSize(0, 0))
        {
            int fileNamePos = filePath.find_last_of("/\\");
            lastOpenSaveFileDirectory = QString::fromStdString(filePath.substr(0, fileNamePos));
            std::string fileName = filePath.substr(fileNamePos + 1);
            int extPos = filePath.find_last_of(".");
            std::string ext = filePath.substr(extPos + 1);
            std::string upperExt = ext;
            std::transform(ext.begin(), ext.end(), upperExt.begin(), toupper);
            if (image.getIsNativeIMG(fileName) || upperExt == "SET" || image.getPaletteName() == "Integrated")
            {
                // Special case when native palette of IMG is integrated but file is set to not
                if (image.getIsNativeIMG(fileName) == true && image.getPaletteName() == "Integrated" && (image.getImageFlags() & 0x0100) == 0x0000)
                {
                    QMessageBox *askForPalette = new QMessageBox(this);
                    askForPalette->setWindowTitle("Choose color palette");
                    askForPalette->setText(QString::fromStdString(fileName) + " is a Arena native IMG file but has not the integrated palette it should have.\n"
                                           "This program default is to set the palette to PAL.COL, but you can choose an other one.\n\n"
                                           "What do you want to do?");
                    QAbstractButton *PALButton = askForPalette->addButton(QString("Use PAL.COL"), QMessageBox::AcceptRole);
                    QAbstractButton *customButton = askForPalette->addButton(QString("Use an other"), QMessageBox::RejectRole);
                    askForPalette->exec();
                    if (askForPalette->clickedButton() == PALButton)
                    {
                        std::string saveFilePath(filePath);
                        saveFilePath.replace(extPos, filePath.size()-extPos, ".PNG");
                        Img.save(QString::fromStdString(saveFilePath), "png");
                    }
                    else if (askForPalette->clickedButton() == customButton)
                    {
                        std::ifstream paletteStream;
                        QString qPalettePath = QFileDialog::getOpenFileName(this, "Choose a palette file to open", lastColDirectory, "COL files (*.col)");
                        std::string palettePath = qPalettePath.toStdString();
                        int fileNamePos = palettePath.find_last_of("/\\");
                        lastColDirectory = QString::fromStdString(palettePath.substr(0, fileNamePos));
                        paletteStream.open(palettePath, std::ios_base::in | std::ios_base::binary);
                        if (paletteStream.is_open())
                        {
                            paletteStream.seekg(0, std::ios_base::end);
                            size_t paletteSize = paletteStream.tellg();
                            if (paletteSize == 776)
                            {
                                customPalette.clear();
                                uint8_t R(0), G(0), B(0);
                                paletteStream.seekg(8, std::ios_base::beg);
                                for (size_t i(0); i<256; i++)
                                {
                                    paletteStream.read(reinterpret_cast<char*>(&R), 1);
                                    paletteStream.read(reinterpret_cast<char*>(&G), 1);
                                    paletteStream.read(reinterpret_cast<char*>(&B), 1);
                                    customPalette.append(qRgb(R, G, B));
                                }
                                Img.setColorTable(customPalette);
                                std::string saveFilePath(filePath);
                                saveFilePath.replace(extPos, filePath.size()-extPos, ".PNG");
                                Img.save(QString::fromStdString(saveFilePath), "png");

                            }
                            else
                            {
                                QMessageBox::warning(this, "Open custom color palette", "this is not a valid palette file.");
                            }
                        }
                        else
                        {
                            QMessageBox::warning(this, "Open custom color palette", "Error while opening palette file.");
                        }
                    }
                }
                else
                {
                    std::string saveFilePath(filePath);
                    saveFilePath.replace(extPos, filePath.size()-extPos, ".PNG");
                    Img.save(QString::fromStdString(saveFilePath), "png");
                }
            }
            else
            {
                if (!setAll)
                {
                    QMessageBox *askForPalette = new QMessageBox(this);
                    askForPalette->setWindowTitle("Choose color palette");
                    askForPalette->setText(QString::fromStdString(fileName) + " is not an Arena native IMG file and has no integrated palette.\n"
                                           "This program default is to set the palette to PAL.COL, but you can choose an other one.\n\n"
                                           "What do you want to do?");
                    QAbstractButton *PALButton = askForPalette->addButton(QString("Use PAL.COL"), QMessageBox::AcceptRole);
                    QAbstractButton *PALForAllButton = askForPalette->addButton(QString("Use PAL.COL for All"), QMessageBox::AcceptRole);
                    QAbstractButton *customButton = askForPalette->addButton(QString("Use an other"), QMessageBox::RejectRole);
                    QAbstractButton *customForAllButton = askForPalette->addButton(QString("Use an other for All"), QMessageBox::RejectRole);
                    if (nbrOfIMGs <= 1)
                    {
                        askForPalette->removeButton(PALForAllButton);
                        askForPalette->removeButton(customForAllButton);
                    }
                    askForPalette->exec();
                    if (askForPalette->clickedButton() == PALButton || askForPalette->clickedButton() == PALForAllButton)
                    {
                        if (askForPalette->clickedButton() == PALForAllButton)
                        {
                            setAll = true;
                        }
                        std::string saveFilePath(filePath);
                        saveFilePath.replace(extPos, filePath.size()-extPos, ".PNG");
                        Img.save(QString::fromStdString(saveFilePath), "png");
                    }
                    else if (askForPalette->clickedButton() == customButton || askForPalette->clickedButton() == customForAllButton)
                    {
                        if (askForPalette->clickedButton() == customForAllButton)
                        {
                            setAll = true;
                        }
                        std::ifstream paletteStream;
                        QString qPalettePath = QFileDialog::getOpenFileName(this, "Choose a palette file to open", lastColDirectory, "COL files (*.col)");
                        std::string palettePath = qPalettePath.toStdString();
                        int fileNamePos = palettePath.find_last_of("/\\");
                        lastColDirectory = QString::fromStdString(palettePath.substr(0, fileNamePos));
                        paletteStream.open(palettePath, std::ios_base::in | std::ios_base::binary);
                        if (paletteStream.is_open())
                        {
                            paletteStream.seekg(0, std::ios_base::end);
                            size_t paletteSize = paletteStream.tellg();
                            if (paletteSize == 776)
                            {
                                customPalette.clear();
                                uint8_t R(0), G(0), B(0);
                                paletteStream.seekg(8, std::ios_base::beg);
                                for (size_t i(0); i<256; i++)
                                {
                                    paletteStream.read(reinterpret_cast<char*>(&R), 1);
                                    paletteStream.read(reinterpret_cast<char*>(&G), 1);
                                    paletteStream.read(reinterpret_cast<char*>(&B), 1);
                                    customPalette.append(qRgb(R, G, B));
                                }
                                Img.setColorTable(customPalette);
                                std::string saveFilePath(filePath);
                                saveFilePath.replace(extPos, filePath.size()-extPos, ".PNG");
                                Img.save(QString::fromStdString(saveFilePath), "png");

                            }
                            else
                            {
                                setAll = false;
                                QMessageBox::warning(this, "Open custom color palette", "this is not a valid palette file.");
                            }
                        }
                        else
                        {
                            setAll = false;
                            QMessageBox::warning(this, "Open custom color palette", "Error while opening palette file.");
                        }
                    }
                }
                else
                {
                    if (choice == "PAL.COL")
                    {
                        std::string saveFilePath(filePath);
                        saveFilePath.replace(extPos, filePath.size()-extPos, ".PNG");
                        Img.save(QString::fromStdString(saveFilePath), "png");
                    }
                    else
                    {
                        Img.setColorTable(customPalette);
                        std::string saveFilePath(filePath);
                        saveFilePath.replace(extPos, filePath.size()-extPos, ".PNG");
                        Img.save(QString::fromStdString(saveFilePath), "png");
                    }
                }
            }
        }
        else
        {
            QMessageBox::warning(this, "External convertion", sourceFilesPath[i] + " is not a valid IMG/SET file.");
        }
    }
}

// Convert external PNG(s) to IMG/SET
void MainWindow::convertPNGToIMGSETSlot()
{
    QStringList sourceFilesPath = QFileDialog::getOpenFileNames(this, "Choose PNG file(s) to convert", lastOpenSaveFileDirectory, "PNG files (*.png)");
    int pngNbr = sourceFilesPath.size();    

    // For use with non native conversion
    QString fileType = "IMG";
    bool integratePalette(false), setForAll(false);
    uint16_t offsetX(0), offsetY(0);

    for (int i(0); i<pngNbr; i++)
    {
        std::ofstream ofstream;

        // Building some filenames and paths (PNG, IMG and SET)
        std::string sourceFilePath = sourceFilesPath[i].toStdString();
        int fileNamePos = sourceFilePath.find_last_of("/\\");
        lastOpenSaveFileDirectory = QString::fromStdString(sourceFilePath.substr(0, fileNamePos));
        std::string sourceFileName = sourceFilePath.substr(fileNamePos + 1);
        int extPos = sourceFilePath.find_last_of(".");
        std::string ext = sourceFilePath.substr(extPos + 1);
        std::string upperExt = ext;
        std::transform(ext.begin(), ext.end(), upperExt.begin(), toupper);
        std::string saveFilePathIMG(sourceFilePath);
        saveFilePathIMG.replace(extPos, sourceFilePath.size()-extPos, ".IMG");
        std::string saveFileNameIMG = saveFilePathIMG.substr(fileNamePos + 1);
        std::string saveFilePathSET(sourceFilePath);
        saveFilePathSET.replace(extPos, sourceFilePath.size()-extPos, ".SET");
        std::string saveFileNameSET = saveFilePathSET.substr(fileNamePos + 1);

        // Check source file extension
        if (upperExt == "PNG")
        {
            std::ifstream nativeIMGHeader;
            nativeIMGHeader.open("data/nativeIMGHeader", std::ios_base::in | std::ios_base::binary);
            // Check BSATool data have been opened
            if (nativeIMGHeader.is_open())
            {
                QImage img(QString::fromStdString(sourceFilePath));
                // Check built QImage size
                if (img.size() != QSize(0,0))
                {
                    // Image is 8bit indexed
                    if (img.format() == QImage::Format_Indexed8 && img.colorTable().size() == 256)
                    {
                        // save filename is one of the native IMG
                        if (image.getIsNativeIMG(saveFileNameIMG))
                        {
                            std::string paletteType = image.getNativeIMGPaletteType(saveFileNameIMG);
                            // Check if native IMG has no header
                            if (paletteType == "texture" || paletteType == "noHeader")
                            {
                                // If IMG is a texture one
                                if (paletteType == "texture")
                                {
                                    // If image has not the native dimensions
                                    if (img.height() != 64 || img.width() != 64)
                                    {
                                        int ret = QMessageBox::question(this, "PNG to IMG convertion", "Arena's default for texture is 64x64 image."
                                                                        " This file has not the right dimensions, "
                                                                        "IMG will be weird and Arena will problably not open it correctly.\n"
                                                                        " Are you sure you want to create the IMG?");
                                        // User want to build the IMG anyway
                                        if (ret == QMessageBox::Yes)
                                        {
                                            ofstream.open(saveFilePathIMG, std::ios_base::out | std::ios_base::binary);
                                            // Check if ofstream is opened
                                            if (ofstream.is_open())
                                            {
                                                for (int i(0); i<img.height(); i++)
                                                {
                                                    const uchar  *pixeldata = img.constScanLine(i);
                                                    ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                                }
                                            }
                                            // Ofstream not opened
                                            else
                                            {
                                                QMessageBox::warning(this, "PNG to IMG convertion", "Error while opening " +
                                                                     QString::fromStdString(saveFileNameIMG) + " for writting.");
                                            }
                                        }
                                    }
                                    // Image has the native dimensions
                                    else
                                    {
                                        ofstream.open(saveFilePathIMG, std::ios_base::out | std::ios_base::binary);
                                        // Check if ofstream is opened
                                        if (ofstream.is_open())
                                        {
                                            for (int i(0); i<64; i++)
                                            {
                                                const uchar  *pixeldata = img.constScanLine(i);
                                                ofstream.write(reinterpret_cast<const char*>(pixeldata), 64);
                                            }
                                        }
                                        // Ofstream not opened
                                        else
                                        {
                                            QMessageBox::warning(this, "PNG to IMG convertion", "Error while opening " +
                                                                 QString::fromStdString(saveFileNameIMG) + " for writting.");
                                        }
                                    }
                                }
                                // Image is a noHeaderIMG
                                else
                                {
                                    int nativeWidth = image.getNoHeaderIMGWidth(saveFileNameIMG);
                                    int nativeHeight = image.getNoHeaderIMGHeight(saveFileNameIMG);
                                    // If image has not the native dimensions
                                    if (img.height() != nativeHeight || img.width() != nativeWidth)
                                    {
                                        int ret = QMessageBox::question(this, "PNG to IMG convertion", "Arena's default for this image is " +
                                                                        QString::number(nativeWidth) + "x" + QString::number(nativeHeight) + ". "
                                                                        "This file has not the right dimensions, "
                                                                        "IMG will be weird and Arena will problably not open it correctly.\n"
                                                                        " Are you sure you want to create the IMG?");
                                        // User want to build the IMG anyway
                                        if (ret == QMessageBox::Yes)
                                        {
                                            ofstream.open(saveFilePathIMG, std::ios_base::out | std::ios_base::binary);
                                            // Check if ofstream is opened
                                            if (ofstream.is_open())
                                            {
                                                for (int i(0); i<img.height(); i++)
                                                {
                                                    const uchar  *pixeldata = img.constScanLine(i);
                                                    ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                                }
                                            }
                                            // Ofstream not opened
                                            else
                                            {
                                                QMessageBox::warning(this, "PNG to IMG convertion", "Error while opening " +
                                                                     QString::fromStdString(saveFileNameIMG) + " for writting.");
                                            }
                                        }
                                    }
                                    // Image has the native dimensions
                                    else
                                    {
                                        ofstream.open(saveFilePathIMG, std::ios_base::out | std::ios_base::binary);
                                        // Check if ofstream is opened
                                        if (ofstream.is_open())
                                        {
                                            for (int i(0); i<nativeHeight; i++)
                                            {
                                                const uchar  *pixeldata = img.constScanLine(i);
                                                ofstream.write(reinterpret_cast<const char*>(pixeldata), nativeWidth);
                                            }
                                        }
                                        // Ofstream not opened
                                        else
                                        {
                                            QMessageBox::warning(this, "PNG to IMG convertion", "Error while opening " +
                                                                 QString::fromStdString(saveFileNameIMG) + " for writting.");
                                        }
                                    }
                                }
                            }
                            // Native IMG with a header
                            else
                            {
                                char *fileNameBuffer = new char[14];
                                uint32_t natOffsets(0);
                                uint16_t natWidth(0), natHeight(0), natFlags(0), natDataSize(0);
                                for (int i(0); i<476; i++)
                                {
                                    nativeIMGHeader.seekg(i*26);
                                    nativeIMGHeader.read(fileNameBuffer, 14);
                                    if (saveFileNameIMG == fileNameBuffer)
                                    {
                                        nativeIMGHeader.read(reinterpret_cast<char*>(&natOffsets), 4);
                                        nativeIMGHeader.read(reinterpret_cast<char*>(&natWidth), 2);
                                        nativeIMGHeader.read(reinterpret_cast<char*>(&natHeight), 2);
                                        nativeIMGHeader.read(reinterpret_cast<char*>(&natFlags), 2);
                                        nativeIMGHeader.read(reinterpret_cast<char*>(&natDataSize), 2);
                                    }
                                }
                                delete[] fileNameBuffer;
                                // If image and native IMG have different dimensions
                                if (img.width() != natWidth || img.height() != natHeight)
                                {
                                    int ret = QMessageBox::question(this, "PNG to IMG convertion", "Arena's default for this image is " +
                                                                    QString::number(natWidth) + "x" + QString::number(natHeight) + ". "
                                                                    "This file has not the right dimensions, "
                                                                    "Chances are that Arena does not display it correctly.\n"
                                                                    " Are you sure you want to create the IMG?\n"
                                                                    "(If yes, think to check the offset X and Y of the IMG you will create as "
                                                                    "this program will write the same offsets than the native IMG)");
                                    // User want to build the IMG anyway
                                    if (ret == QMessageBox::Yes)
                                    {
                                        ofstream.open(saveFilePathIMG, std::ios_base::out | std::ios_base::binary);
                                        // Check if ofstream is opened
                                        if (ofstream.is_open())
                                        {
                                            ofstream.write(reinterpret_cast<const char*>(&natOffsets), 4);
                                            uint16_t newWidth(img.width()), newHeight(img.height());
                                            ofstream.write(reinterpret_cast<char*>(&newWidth), 2);
                                            ofstream.write(reinterpret_cast<char*>(&newHeight), 2);
                                            // The IMG created will be decompressed -> compression flag set to zero
                                            uint16_t newFlags = natFlags & 0xFF00;
                                            ofstream.write(reinterpret_cast<char*>(&newFlags), 2);
                                            uint16_t newDataSize(newWidth*newHeight);
                                            ofstream.write(reinterpret_cast<char*>(&newDataSize), 2);
                                            for (int i(0); i<newHeight; i++)
                                            {
                                                const uchar  *pixeldata = img.constScanLine(i);
                                                ofstream.write(reinterpret_cast<const char*>(pixeldata), newWidth);
                                            }
                                            // If native IMG has an integrated palette, it is added at the end of the file
                                            if ((natFlags & 0xFF00) == 0x0100)
                                            {
                                                QVector<QRgb> imgPalette = img.colorTable();
                                                uint8_t R(0), G(0), B(0);
                                                for (int i(0); i<256; i++)
                                                {
                                                    QRgb color = imgPalette[i];
                                                    R = qRed(color);
                                                    R = R >> 2;
                                                    G = qGreen(color);
                                                    G = G >> 2;
                                                    B = qBlue(color);
                                                    B = B >> 2;
                                                    ofstream.write(reinterpret_cast<char*>(&R), 1);
                                                    ofstream.write(reinterpret_cast<char*>(&G), 1);
                                                    ofstream.write(reinterpret_cast<char*>(&B), 1);
                                                }
                                            }
                                        }
                                        // Ofstream not opened
                                        else
                                        {
                                            QMessageBox::warning(this, "PNG to IMG convertion", "Error while opening " +
                                                                 QString::fromStdString(saveFileNameIMG) + " for writting.");
                                        }
                                    }
                                }
                                // Image and native have the same dimensions
                                else
                                {
                                    ofstream.open(saveFilePathIMG, std::ios_base::out | std::ios_base::binary);
                                    // Check if ofstream is opened
                                    if (ofstream.is_open())
                                    {
                                        ofstream.write(reinterpret_cast<const char*>(&natOffsets), 4);
                                        ofstream.write(reinterpret_cast<char*>(&natWidth), 2);
                                        ofstream.write(reinterpret_cast<char*>(&natHeight), 2);
                                        // The IMG created will be decompressed -> compression flag set to zero
                                        uint16_t newFlags = natFlags & 0xFF00;
                                        ofstream.write(reinterpret_cast<char*>(&newFlags), 2);
                                        uint16_t newDataSize(natWidth*natHeight);
                                        ofstream.write(reinterpret_cast<char*>(&newDataSize), 2);
                                        for (int i(0); i<natHeight; i++)
                                        {
                                            const uchar  *pixeldata = img.constScanLine(i);
                                            ofstream.write(reinterpret_cast<const char*>(pixeldata), natWidth);
                                        }
                                        // If native IMG has an integrated palette, it is added at the end of the file
                                        if ((natFlags & 0xFF00) == 0x0100)
                                        {
                                            QVector<QRgb> imgPalette = img.colorTable();
                                            uint8_t R(0), G(0), B(0);
                                            for (int i(0); i<256; i++)
                                            {
                                                QRgb color = imgPalette[i];
                                                R = qRed(color);
                                                R = R >> 2;
                                                G = qGreen(color);
                                                G = G >> 2;
                                                B = qBlue(color);
                                                B = B >> 2;
                                                ofstream.write(reinterpret_cast<char*>(&R), 1);
                                                ofstream.write(reinterpret_cast<char*>(&G), 1);
                                                ofstream.write(reinterpret_cast<char*>(&B), 1);
                                            }
                                        }
                                    }
                                    // Ofstream not opened
                                    else
                                    {
                                        QMessageBox::warning(this, "PNG to IMG convertion", "Error while opening " +
                                                             QString::fromStdString(saveFileNameIMG) + " for writting.");
                                    }
                                }
                            }
                        }
                        // Save filename is one of the native SET
                        else if (image.getIsNativeSET(saveFileNameSET))
                        {
                            int nativeWidth(image.getNativeSETWidth(saveFileNameSET));
                            int nativeHeight(image.getNativeSETHeight(saveFileNameSET));
                            // If image and native have different dimensions
                            if (img.width() != nativeWidth || img.height() != nativeHeight)
                            {
                                int ret = QMessageBox::question(this, "PNG to SET convertion", "Arena's default for this SET image is " +
                                                                QString::number(nativeWidth) + "x" + QString::number(nativeHeight) + ". "
                                                                "This file has not the right dimensions, "
                                                                "Chances are that Arena will not display it correctly.\n"
                                                                " Are you sure you want to create the SET?");
                                // User want to build the SET anyway
                                if (ret == QMessageBox::Yes)
                                {
                                    ofstream.open(saveFilePathSET, std::ios_base::out | std::ios_base::binary);
                                    // Check if ofstream is opened
                                    if (ofstream.is_open())
                                    {
                                        for (int i(0); i<img.height(); i++)
                                        {
                                            const uchar  *pixeldata = img.constScanLine(i);
                                            ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                        }
                                    }
                                    // Ofstream not opened
                                    else
                                    {
                                        QMessageBox::warning(this, "PNG to SET convertion", "Error while opening " +
                                                             QString::fromStdString(saveFileNameSET) + " for writting.");
                                    }
                                }
                            }
                            // Image has the native dimensions
                            else
                            {
                                ofstream.open(saveFilePathSET, std::ios_base::out | std::ios_base::binary);
                                // Check if ofstream is opened
                                if (ofstream.is_open())
                                {
                                    for (int i(0); i<img.height(); i++)
                                    {
                                        const uchar  *pixeldata = img.constScanLine(i);
                                        ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                    }
                                }
                                // Ofstream not opened
                                else
                                {
                                    QMessageBox::warning(this, "PNG to SET convertion", "Error while opening " +
                                                         QString::fromStdString(saveFileNameSET) + " for writting.");
                                }
                            }
                        }
                        // File is not a native SET or IMG (by checking name at least!) Ask user for informations
                        else
                        {
                            // Ask user info if not set for all other file on a previous asking
                            if (!setForAll)
                            {
                                AskUserWindow *askUserWindow = new AskUserWindow(QString::fromStdString(sourceFileName));
                                int textureIndex = askUserWindow->fileExtComboBox->findText("Texture (64*64 IMG)");
                                if (img.width() != 64 || img.height() != 64)
                                {
                                    askUserWindow->fileExtComboBox->removeItem(textureIndex);
                                }
                                int ret = askUserWindow->exec();
                                // User want to create the file
                                if (ret == QDialog::Accepted)
                                {
                                    fileType = askUserWindow->getFileType();
                                    offsetX = askUserWindow->getOffsetX();
                                    offsetY = askUserWindow->getOffsetY();
                                    integratePalette = askUserWindow->getIntegratePalette();
                                    setForAll = askUserWindow->getSettingForAll();
                                    if (fileType == "IMG" || fileType == "Texture (64*64 IMG)")
                                    {
                                        ofstream.open(saveFilePathIMG, std::ios_base::out | std::ios_base::binary);
                                    }
                                    else
                                    {
                                        ofstream.open(saveFilePathSET, std::ios_base::out | std::ios_base::binary);
                                    }
                                    // Check if ofstream is opened
                                    if (ofstream.is_open())
                                    {
                                        // User want a SET file
                                        if (fileType == "SET")
                                        {
                                            for (int i(0); i<img.height(); i++)
                                            {
                                                const uchar  *pixeldata = img.constScanLine(i);
                                                ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                            }
                                        }
                                        // User want a IMG file
                                        else if (fileType == "IMG")
                                        {
                                            ofstream.write(reinterpret_cast<const char*>(&offsetX), 2);
                                            ofstream.write(reinterpret_cast<const char*>(&offsetY), 2);
                                            uint16_t newWidth(img.width()), newHeight(img.height());
                                            ofstream.write(reinterpret_cast<char*>(&newWidth), 2);
                                            ofstream.write(reinterpret_cast<char*>(&newHeight), 2);
                                            // The IMG created will be decompressed -> compression flag set to zero, palette set or not according to user choice
                                            uint16_t newFlags(0x0000);
                                            if (integratePalette)
                                            {
                                                newFlags = 0x0100;
                                            }
                                            ofstream.write(reinterpret_cast<char*>(&newFlags), 2);
                                            uint16_t newDataSize(newWidth*newHeight);
                                            ofstream.write(reinterpret_cast<char*>(&newDataSize), 2);
                                            for (int i(0); i<img.height(); i++)
                                            {
                                                const uchar  *pixeldata = img.constScanLine(i);
                                                ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                            }
                                            if (integratePalette)
                                            {
                                                QVector<QRgb> imgPalette = img.colorTable();
                                                uint8_t R(0), G(0), B(0);
                                                for (int i(0); i<256; i++)
                                                {
                                                    QRgb color = imgPalette[i];
                                                    R = qRed(color);
                                                    R = R >> 2;
                                                    G = qGreen(color);
                                                    G = G >> 2;
                                                    B = qBlue(color);
                                                    B = B >> 2;
                                                    ofstream.write(reinterpret_cast<char*>(&R), 1);
                                                    ofstream.write(reinterpret_cast<char*>(&G), 1);
                                                    ofstream.write(reinterpret_cast<char*>(&B), 1);
                                                }
                                            }
                                        }
                                        // User want a texture IMG
                                        else
                                        {
                                            for (int i(0); i<img.height(); i++)
                                            {
                                                const uchar  *pixeldata = img.constScanLine(i);
                                                ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                            }
                                        }
                                    }
                                    // Ofstream not opened
                                    else
                                    {
                                        QMessageBox::warning(this, "PNG to SET convertion", "Error while opening " +
                                                             QString::fromStdString(saveFileNameSET) + " for writting.");
                                    }
                                }
                            }
                            // User gave infos for all other non native conversions
                            else
                            {
                                bool needToReask(false);
                                if (fileType == "IMG")
                                {
                                    ofstream.open(saveFilePathIMG, std::ios_base::out | std::ios_base::binary);
                                }
                                else if (fileType == "SET")
                                {
                                    ofstream.open(saveFilePathSET, std::ios_base::out | std::ios_base::binary);
                                }
                                // User want a texture IMG for all other. Need to check the file first
                                else
                                {
                                    if (img.width() != 64 || img.height() != 64)
                                    {
                                        needToReask = true;
                                    }
                                    else
                                    {
                                        ofstream.open(saveFilePathIMG, std::ios_base::out | std::ios_base::binary);
                                    }
                                }
                                // Check if ofstream is opened
                                if (ofstream.is_open() || needToReask)
                                {
                                    // User want a SET file
                                    if (fileType == "SET")
                                    {
                                        for (int i(0); i<img.height(); i++)
                                        {
                                            const uchar  *pixeldata = img.constScanLine(i);
                                            ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                        }
                                    }
                                    // User want a IMG file
                                    else if (fileType == "IMG")
                                    {
                                        ofstream.write(reinterpret_cast<const char*>(&offsetX), 2);
                                        ofstream.write(reinterpret_cast<const char*>(&offsetY), 2);
                                        uint16_t newWidth(img.width()), newHeight(img.height());
                                        ofstream.write(reinterpret_cast<char*>(&newWidth), 2);
                                        ofstream.write(reinterpret_cast<char*>(&newHeight), 2);
                                        // The IMG created will be decompressed -> compression flag set to zero, palette set or not according to user choice
                                        uint16_t newFlags(0x0000);
                                        if (integratePalette)
                                        {
                                            newFlags = 0x0100;
                                        }
                                        ofstream.write(reinterpret_cast<char*>(&newFlags), 2);
                                        uint16_t newDataSize(newWidth*newHeight);
                                        ofstream.write(reinterpret_cast<char*>(&newDataSize), 2);
                                        for (int i(0); i<img.height(); i++)
                                        {
                                            const uchar  *pixeldata = img.constScanLine(i);
                                            ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                        }
                                        if (integratePalette)
                                        {
                                            QVector<QRgb> imgPalette = img.colorTable();
                                            uint8_t R(0), G(0), B(0);
                                            for (int i(0); i<256; i++)
                                            {
                                                QRgb color = imgPalette[i];
                                                R = qRed(color);
                                                R = R >> 2;
                                                G = qGreen(color);
                                                G = G >> 2;
                                                B = qBlue(color);
                                                B = B >> 2;
                                                ofstream.write(reinterpret_cast<char*>(&R), 1);
                                                ofstream.write(reinterpret_cast<char*>(&G), 1);
                                                ofstream.write(reinterpret_cast<char*>(&B), 1);
                                            }
                                        }
                                    }
                                    // User want a texture IMG
                                    else
                                    {
                                        // send a message if opereration impossible and ask user again for this file
                                        if (needToReask)
                                        {
                                            QMessageBox::warning(this, "PNG to texture IMG conversion", "Choice has been made to convert all non native to texture IMG,"
                                                                 " but the size of this file is not 64x64. Please make a new choice.");
                                            setForAll = false;
                                            i --;
                                        }
                                        else
                                        {
                                            for (int i(0); i<img.height(); i++)
                                            {

                                                const uchar  *pixeldata = img.constScanLine(i);
                                                ofstream.write(reinterpret_cast<const char*>(pixeldata), img.width());
                                            }
                                        }
                                    }
                                }
                                // Ofstream not opened
                                else
                                {
                                    QMessageBox::warning(this, "PNG to SET convertion", "Error while opening " +
                                                         QString::fromStdString(saveFileNameSET) + " for writting.");
                                }
                            }
                        }
                    }
                    // If image format is not 8bit indexed
                    else
                    {
                        QMessageBox::warning(this, "PNG to IMG convertion",
                                             "Error during conversion : the file " + QString::fromStdString(sourceFileName) + " was not a 8bit indexed PNG,"
                                             " or the color palette has a color number different than 256.");
                    }
                }
                // Qimage built has size 0
                else
                {
                    QMessageBox::warning(this, "PNG to IMG convertion", "Unknown error during conversion, no valid IMG could be constructed.");
                }
            }
            // BSATool data could not be opened
            else
            {
                QMessageBox::warning(this, "PNG to IMG convertion", "Error while opening 'data/nativeIMGHeader' for reading.");
            }
        }
        //source extension not png
        else
        {
            QMessageBox::warning(this, "PNG to IMG convertion", sourceFilesPath[i] + " is not a valid PNG file.");
        }
    }
}

// Update file(s) in archive data with other(s)
void MainWindow::updateFileSlot()
{
    QStringList filesPath = QFileDialog::getOpenFileNames(this, "Choose update file(s)", lastOpenSaveFileDirectory);
    for (int i(0); i<filesPath.size(); i++)
    {
        std::string sourceFilePath = filesPath[i].toStdString();
        int sourceFileNamePos = sourceFilePath.find_last_of("/\\");
        std::string sourceFileName = sourceFilePath.substr(sourceFileNamePos + 1);
        bool isPresent(false);
        for (int j(0); j<filelist.size(); j++)
        {
             if (filelist[j] == QString::fromStdString(sourceFileName))
             {
                 isPresent = true;
             }
        }
        if (isPresent)
        {
            int index = BSAFile::getInstance()->getIndex(sourceFileName);
            BSAFile::getInstance()->updateFile(index, sourceFilePath);
        }
        else
        {
            QMessageBox::information(this, "File update", "the file " + QString::fromStdString(sourceFileName) +
                                     " is not in archive. Please use add files instead of update files.");
        }
    }
    QString currentFilter = fileFilter->currentText();
    int currentRow = archiveFileList->currentRow();
    UpdateShowedFileList(currentFilter);
    archiveFileList->setCurrentRow(currentRow);
}

// Delete a file in archive data
void MainWindow::deleteFileSlot()
{
    QListWidgetItem *item = archiveFileList->currentItem();
    item->setBackgroundColor(QColor(Qt::red));
    QString fileName = item->text();
    int index = BSAFile::getInstance()->getIndex(fileName.toStdString());
    BSAFile::getInstance()->deleteFile(index);
    updatePreview(item);
    updateInfoLabel(item);
    updateQActionsState(item);
}

// Add file(s) to archive data
void MainWindow::addFileSlot()
{
    QStringList filesPath = QFileDialog::getOpenFileNames(this, "Choose file(s) to add", lastOpenSaveFileDirectory);
    for (int i(0); i<filesPath.size(); i++)
    {
        std::string sourceFilePath = filesPath[i].toStdString();
        int sourceFileNamePos = sourceFilePath.find_last_of("/\\");
        std::string sourceFileName = sourceFilePath.substr(sourceFileNamePos + 1);
        if (sourceFileName.size() > 13)
        {
            QMessageBox::information(this, "Add file", "the file " + QString::fromStdString(sourceFileName) +
                                     " is too long. Due to limitations in Arena BSA format, filename maximum size is 13 characters.");
        }
        else
        {
            bool isPresent(false);
            for (int j(0); j<filelist.size(); j++)
            {
                 if (filelist[j] == QString::fromStdString(sourceFileName))
                 {
                     isPresent = true;
                 }
            }
            if (isPresent)
            {
                QMessageBox::information(this, "Add file", "the file " + QString::fromStdString(sourceFileName) +
                                         " is in archive. Please use update files instead of add files.");
            }
            else
            {
                BSAFile::getInstance()->addFile(sourceFilePath);
            }
        }
    }
    QString currentFilter = fileFilter->currentText();
    int currentRow = archiveFileList->currentRow();
    clearing = true;
    filelist.clear();
    FileFilterList.clear();
    mapFilteredList.clear();
    fileFilter->clear();
    clearing = false;
    generateFileLists();
    fileFilter->addItems(FileFilterList);
    // Just in case it is a new created archive with no file yet
    if (currentFilter != "")
    {
        fileFilter->setCurrentText(currentFilter);
        UpdateShowedFileList(currentFilter);
        archiveFileList->setCurrentRow(currentRow);
    }
    else
    {
        fileFilter->setCurrentText("None");
        UpdateShowedFileList(QString::fromStdString("None"));
    }
}

// Create a new empty bsa
void MainWindow::newBsaSlot()
{
    bool creating (false);
    if (BSAFile::getInstance()->getIsOpened())
    {
        int ret = QMessageBox::question(this, "Create a new BSA", "A BSA archive already exists.\n"
                                        "If you create a new one, unsaved changes of the previous BSA will be discarded.\n"
                                        "Do you really want to create a new BSA archive?");
        if (ret == QMessageBox::Yes)
        {
            creating = true;
        }
    }
    if (!BSAFile::getInstance()->getIsOpened() || creating == true)
    {
        if (creating == true)
        {
            clearing = true;
            clear();
            clearing = false;
            creating = false;
        }
        BSAFile::getInstance()->createArchive();
        updateQActionsState();
        infoText = "<table><tr><td><b>Bsa archive:</b><td><tr><td>Name:<td>New BSA<tr><td>Header:<td>0 files<tr><td>Size:<td>0 bytes"
                "<tr><td><b>File:</b><td><tr><td>Name:<td><tr><td>Index:<td><tr><td>Type:<td><tr><td>Flags:<td><tr><td>Image size:<td>"
                "<tr><td>Palette:<td><tr><td>Compression:<td><tr><td>Size:<td><tr><td>Offset:<td></table>";
        infoLabel->setText(infoText);
    }
}

// Cancel delete file
void MainWindow::cancelDeleteFileSlot()
{
    QListWidgetItem *item = archiveFileList->currentItem();
    QString fileName = item->text();
    int index = BSAFile::getInstance()->getIndex(fileName.toStdString());
    if (BSAFile::getInstance()->getIsFileDelete(index))
    {
        BSAFile::getInstance()->cancelDeleteFile(index);
        QListWidgetItem *currentItem = archiveFileList->currentItem();
        if (BSAFile::getInstance()->getIsFileUpdated(index))
        {
            currentItem->setBackgroundColor(QColor(Qt::blue));
        }
        else if (BSAFile::getInstance()->getIsFileNew(index))
        {
            currentItem->setBackgroundColor(QColor(Qt::green));
        }
        else
        {
            currentItem->setBackgroundColor(QColor(Qt::white));
        }
        updatePreview(currentItem);
        updateInfoLabel(currentItem);
        updateQActionsState(currentItem);
    }
}

// Cancel update file
void MainWindow::cancelUpdateFileSlot()
{
    QListWidgetItem *item = archiveFileList->currentItem();
    QString fileName = item->text();
    int index = BSAFile::getInstance()->getIndex(fileName.toStdString());
    if (BSAFile::getInstance()->getIsFileUpdated(index))
    {
        BSAFile::getInstance()->cancelUpdateFile(index);
        QListWidgetItem *currentItem = archiveFileList->currentItem();
        if (BSAFile::getInstance()->getIsFileDelete(index))
        {
            currentItem->setBackgroundColor(QColor(Qt::red));
        }
        else if (BSAFile::getInstance()->getIsFileNew(index))
        {
            currentItem->setBackgroundColor(QColor(Qt::green));
        }
        else
        {
            currentItem->setBackgroundColor(QColor(Qt::white));
        }
        updatePreview(currentItem);
        updateInfoLabel(currentItem);
        updateQActionsState(currentItem);
    }
}

// Save changes in a BSA archive
void MainWindow::saveBSASlot()
{
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save BSA", lastBSADirectory, "BSA archives (*.bsa)");
    QString currentFilter = fileFilter->currentText();
    if (saveFilePath != NULL)
    {
        std::string currentBSAFilePath = BSAFile::getInstance()->getArchiveFilePath();
        int ret;
        if (saveFilePath.toStdString() == currentBSAFilePath)
        {
            std::string tempSaveFilePath = saveFilePath.toStdString() + ".temp";
            int fileNamePos = tempSaveFilePath.find_last_of("/\\");
            lastOpenSaveFileDirectory = QString::fromStdString(tempSaveFilePath.substr(0, fileNamePos));
            int tempSaveFileDirPos = tempSaveFilePath.find_last_of("/\\");
            std::string tempSaveFileDir = tempSaveFilePath.substr(0, tempSaveFileDirPos + 1);
            ret = BSAFile::getInstance()->saveArchiveToBSA(tempSaveFilePath);
            if (ret == 0)
            {
                QDir currentSaveDir(QString::fromStdString(tempSaveFileDir));
                currentSaveDir.remove(saveFilePath);
                currentSaveDir.rename(QString::fromStdString(tempSaveFilePath), saveFilePath);
            }
        }
        else
        {
            ret = BSAFile::getInstance()->saveArchiveToBSA(saveFilePath.toStdString());
        }
        // Return 0: ok, 1: stop because file updated or new could not be opened for reading, 2: stop because save path could not be opened for writing,
        // 3: no write because nbr of delete file = total nbr of file
        switch (ret)
        {
        case 0:
        {
            clearing = true;
            clear();
            BSAFile::getInstance()->clear();
            clearing = false;
            int ret2 = BSAFile::getInstance()->openArchive(saveFilePath.toStdString());
            switch (ret2)
            {
            case 0:
            {
                generateFileLists();
                fileFilter->addItems(FileFilterList);
                UpdateShowedFileList(QString("None"));
                for (int i(0); i<FileFilterList.size(); i++)
                {
                    if (FileFilterList[i] == currentFilter)
                    {
                        fileFilter->setCurrentText(currentFilter);
                        UpdateShowedFileList(currentFilter);
                        archiveFileList->setCurrentRow(0);
                    }
                }
                statusBar()->showMessage("BSA archive has been successfully saved and reloaded.");
                break;
            }
            case 1:
            {
                QMessageBox::warning(this, "BSA loading error", "Unable to reload the saved BSA archive.\nThe archive was either corrupted or not a Arena BSA archive");
                statusBar()->showMessage("Unable to reload the saved BSA archive.");
                break;
            }
            case 2:
            {
                QMessageBox::warning(this, "BSA loading error", "Unable to reload the saved BSA archive.\nAn error occured while opening the archive for reading.");
                statusBar()->showMessage("Unable to reload the saved BSA archive.");
                break;
            }
            }
            break;
        }
        case 1:
        {
            QMessageBox::warning(this, "Save BSA", "Save failed because one new or updated file could not be opened for reading");
            break;
        }
        case 2:
        {
            QMessageBox::warning(this, "Save BSA", "Save failed because save path could not be opened for writing");
            break;
        }
        case 3:
        {
            QMessageBox::warning(this, "Save BSA", "No need to save because all files are to be deleted");
            break;
        }
        }
    }
}

// Decrypt or encrypt INF text file
void MainWindow::encryptDecryptINFSlot()
{
    QStringList filesPath = QFileDialog::getOpenFileNames(this, "Choose INF to crypt/decrypt", lastOpenSaveFileDirectory, "INF files (*.inf)");
    for (int i(0); i<filesPath.size(); i++)
    {
        std::string filePath = filesPath[i].toStdString();
        int fileNamePos = filePath.find_last_of("/\\");
        std::string fileName = filePath.substr(fileNamePos + 1);
        std::fstream fstream;
        fstream.open(filePath, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
        if (fstream.is_open())
        {
            fstream.seekg(0, std::ios_base::end);
            size_t length = fstream.tellg();
            fstream.seekg(0, std::ios_base::beg);
            char *fileData = new char[length];
            fstream.read(fileData, length);
            Compression::encryptDecryptINF(fileData, length);
            fstream.seekg(0, std::ios_base::beg);
            fstream.write(fileData, length);
            delete[] fileData;
        }
        else
        {
            QMessageBox::warning(this, "INF crypt/decryption", "the file " + QString::fromStdString(fileName) + " could not be opened for reading/writing.");
        }
    }
}

//=======================================================================================================
// Test
void MainWindow::test()
{
    std::ifstream ifstream;
    ifstream.open("/home/dysperia/Desktop/TESTSOURCE.IMG", std::ios_base::in | std::ios_base::binary);
    ifstream.seekg(0, std::ios_base::beg);
    uint16_t offsetX, offsetY, width, height, flags, dataSize;
    ifstream.read(reinterpret_cast<char*>(&offsetX), 2);
    ifstream.read(reinterpret_cast<char*>(&offsetY), 2);
    ifstream.read(reinterpret_cast<char*>(&width), 2);
    ifstream.read(reinterpret_cast<char*>(&height), 2);
    ifstream.read(reinterpret_cast<char*>(&flags), 2);
    ifstream.read(reinterpret_cast<char*>(&dataSize), 2);
    uchar *sourceCompData = new uchar[64000];
    uchar *sourceUncompData = new uchar[64000];
    uchar *newCompData = new uchar[64000];
    ifstream.read(reinterpret_cast<char*>(sourceCompData), dataSize);
    Compression::image02Decompression(sourceCompData, sourceUncompData, width, height);
    uint16_t newDataSize = Compression::image02Compression(newCompData, sourceUncompData, width, height);
    std::ofstream ofstream;
    ofstream.open("/home/dysperia/Desktop/TESTNEW.IMG", std::ios_base::out | std::ios_base::binary);
    ofstream.write(reinterpret_cast<char*>(&offsetX), 2);
    ofstream.write(reinterpret_cast<char*>(&offsetY), 2);
    ofstream.write(reinterpret_cast<char*>(&width), 2);
    ofstream.write(reinterpret_cast<char*>(&height), 2);
    ofstream.write(reinterpret_cast<char*>(&flags), 2);
    ofstream.write(reinterpret_cast<char*>(&dataSize), 2);
    ofstream.write(reinterpret_cast<char*>(newCompData), newDataSize);
    delete[] sourceCompData;
    delete[] sourceUncompData;
    delete[] newCompData;
}
//=======================================================================================================

//=======================================================================================================
// Extract header under conditions
void MainWindow::extractHeader()
{
    std::fstream fstream;
    fstream.open("/home/hinomatsuri/Desktop/cifInfo.txt", std::ios_base::out | std::ios_base::binary);
    if (fstream.is_open())
    {
        struct noH
        {
            std::string fileName;
            int width;
            int height;
        };
        noH noHeaderCIF[6];
        noHeaderCIF[0] = {"BRASS.CIF", 8, 8};
        noHeaderCIF[1] = {"BRASS2.CIF", 8, 8};
        noHeaderCIF[2] = {"MARBLE.CIF", 3, 3};
        noHeaderCIF[3] = {"MARBLE2.CIF", 3, 3};
        noHeaderCIF[4] = {"PARCH.CIF", 20, 20};
        noHeaderCIF[5] = {"SCROLL.CIF", 20, 20};
        QStringList list = mapFilteredList["CIF"];
        int size = list.size();
        for (int i=0; i<size; i++)
        {
            std::string name = list[i].toStdString();
            bool hasNoHeader = false;
            for (int k(0); k<6; k++)
            {
                if (name == noHeaderCIF[k].fileName)
                {
                    hasNoHeader = true;
                }
            }
            int index = BSAFile::getInstance()->getIndex(name);
            if (!hasNoHeader)
            {
                fstream.write(name.c_str(), name.size());
                for (size_t j=0; j<(14-name.size()); j++)
                {
                    char c = '\0';
                    fstream.write(&c, 1);
                }
                std::ifstream *stream = BSAFile::getInstance()->getStream();
                stream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
                uint16_t nbFrame=0;
                while (stream->tellg() < (int)(BSAFile::getInstance()->getFileOffset(index)+BSAFile::getInstance()->getFileSize(index)))
                {
                    stream->seekg(10, std::ios_base::cur);
                    uint16_t frameSize;
                    stream->read(reinterpret_cast<char*>(&frameSize), 2);
                    stream->seekg(frameSize, std::ios_base::cur);
                    nbFrame++;
                }
                fstream.write(reinterpret_cast<char*>(&nbFrame), 2);
                stream->seekg(BSAFile::getInstance()->getFileOffset(index), std::ios_base::beg);
                for (int l=0; l<nbFrame; l++)
                {
                    char *header = new char[12];
                    stream->read(header, 12);
                    fstream.write(header, 12);
                    stream->seekg(-2, std::ios_base::cur);
                    uint16_t frameSize;
                    stream->read(reinterpret_cast<char*>(&frameSize), 2);
                    stream->seekg(frameSize, std::ios_base::cur);
                }
            }
        }
    }

}
//=======================================================================================================
