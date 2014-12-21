#include "mainwindow.hpp"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <QToolBar>
#include <QComboBox>
#include <QLineEdit>
#include <QDir>

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Building variable name list
    variableName = {"%a", "%adn", "%amn", "%an", "%apr", "%arc", "%art",
                    "%ba",
                    "%ccs", "%cll", "%cn", "%cn2", "%cp", "%ct",
                    "%da", "%de", "%di", "%dit", "%doc", "%du", "%ds",
                    "%en",
                    "%fn", "%fq",
                    "%g", "%g2", "%g3",
                    "%hc", "%hod",
                    "%i",
                    "%jok",
                    "%lp",
                    "%mi", "%mm" "%mn", "%mpr", "%mt",
                    "%n", "%nap", "%nc", "%ne", "%nh", "%nhd", "%nr", "%nt",
                    "%o", "%oap", "%oc", "%omq", "%opp", "%oth",
                    "%pcf", "%pcn",
                    "%qc", "%qmn", "%qt",
                    "%r", "%ra", "%rcn", "%rf",
                    "%s", "%sn", "%st",
                    "%t", "%ta", "%tan", "%tc", "%tem", "%tg", "%ti", "%tl", "%tq", "%tt",
                    "%u"};

    // Building variable meaning list (in same order than the name list)
    variableMeaning = {"Amount (gold)", "Artifact Dongeon Name", "Artifact Map area Name", "Artifact information giver Name", "Artifact PRovince", "ARtifact Class", "ARTifact name",
                       "Bonus Amount (gold)",
                       "City of Chaos Segment", "Chaos segment Lore Location", "actual City Name", "City Name in status with actual city", "City Province", "City Type",
                       "Date", "Days Exhausted", "DIrection", "Days In Tavern", "Description Of Class", "DUration in days", "Designation of Service buyer",
                       "Equipment store Name",
                       "FullName", "Fullname of Quest giver",
                       "Gender (he, she)", "Gender (him, her)", "Gender (his, her)",
                       "Home City", "HOliday Description",
                       "Item name",
                       "JOKe",
                       "Local (current) Province",
                       "Mission Item", "item price (gold)", "Monster Name", "Map PRovince", "Monster Type",
                       "Name", "Negociated Artifact Price", "Name of Criminal", "Name of Escorted", "Name of Holiday", "Named Holiday Date", "Name of Relative", "Name of Tavern",
                       "Opponent", "Original Artifact Price", "name Of Class", "Object of Move Quest", "OPponent's Player killer", "one of the game interjections",
                       "Player Character Fullname", "Player Character Name",
                       "Quest City", "Quest Monster Name", "Quest Task",
                       "Relative", "RAce", "maybe a wrong city for chaos segment quest", "Ruler Fullname",
                       "Stuff piece name", "Snake Name", "STatus between two cities",
                       "Title (sometime Time in days)", "Task Amount (gold)", "Task Area Name", "Task City", "TEMple", "Task Group", "TIme left in days", "Task Location", "Title of Quest giver", "Transgressor Title",
                       "Units number"};

    // Building the QString to show in the view variable popup (just a big html table from variable name and meaning list)
    variablePopupText = "<table border=1><tr><td><b>Variable name</b><td><b>Meaning</b>";
    for (int i(0); i<variableName.size(); i++)
    {
        variablePopupText += "<tr><td>" + variableName[i] + "<td>" + variableMeaning[i];
    }
    variablePopupText += "</table>";

    // initializing some variable
    isOpened = false;
    isModified = false;
    isSaved = false;
    clearing = false;
    // Main zone
    QWidget *mainZone = new QWidget;
    setCentralWidget(mainZone);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainZone->setLayout(mainLayout);
    entryList = new QListWidget;
    entryList->setMinimumWidth(150);
    entryList->setMaximumWidth(150);
    entryList->setMinimumHeight(400);
    connect(entryList, SIGNAL(currentRowChanged(int)), this, SLOT(changeTextSlot(int)));
    entryEditor = new QPlainTextEdit;
    entryEditor->setPlainText("First open the TEMPLATE.DAT directory.\n\nBoth the files POINTER1.DAT and TEMPLATE.DAT \nmust be present in the same directory.");
    entryEditor->setMinimumWidth(400);
    entryEditor->setWordWrapMode(QTextOption::NoWrap);
    entryEditor->setLineWrapMode(QPlainTextEdit::NoWrap);
    connect(entryEditor, SIGNAL(textChanged()), this, SLOT(saveTextSlot()));
    mainLayout->addWidget(entryList);
    mainLayout->addWidget(entryEditor);

    // Menu bar
    QMenuBar *menuBar = new QMenuBar;
    setMenuBar(menuBar);
    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *openTemplate = fileMenu->addAction("Open TEMLATE.DAT");
    openTemplate->setIcon(QIcon("icon/open_file.png"));
    connect(openTemplate, SIGNAL(triggered()), this, SLOT(openTemplateSlot()));
    saveTemplate = fileMenu->addAction("Save TEMPLATE.DAT");
    saveTemplate->setIcon(QIcon("icon/save_bsa.png"));
    saveTemplate->setDisabled(true);
    connect(saveTemplate, SIGNAL(triggered()), this, SLOT(saveTemplateSlot()));
    QAction *quit = fileMenu->addAction("Exit");
    quit->setIcon(QIcon("icon/exit.png"));
    connect(quit, SIGNAL(triggered()), this, SLOT(close()));
    QMenu *viewMenu = menuBar->addMenu("View");
    QAction *viewVariable = viewMenu->addAction("View variables");
    viewVariable->setIcon(QIcon("icon/view_variable.png"));
    connect(viewVariable, SIGNAL(triggered()), this, SLOT(viewVariableSlot()));
    QMenu *aboutMenu = menuBar->addMenu("About");
    QAction *about = aboutMenu->addAction("About");
    about->setIcon(QIcon("icon/about.png"));
    connect(about, SIGNAL(triggered()), this, SLOT(aboutSlot()));

    // Toolbar
    QToolBar *toolBar = new QToolBar;
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    toolBar->setWindowTitle("Toolbar");
    toolBar->addAction(openTemplate);
    toolBar->addAction(saveTemplate);
    toolBar->addAction(viewVariable);
    toolBar->addSeparator();
    QComboBox *variableComboBox = new QComboBox;
    variableComboBox->addItems(variableName);
    toolBar->addWidget(variableComboBox);
    connect(variableComboBox, SIGNAL(activated(int)), this, SLOT(updateVariableSlot(int)));
    variableLineEdit = new QLineEdit;
    variableLineEdit->setReadOnly(true);
    variableLineEdit->setText(variableMeaning[0]);
    toolBar->addWidget(variableLineEdit);
    toolBar->setIconSize(QSize(36, 36));
    addToolBar(toolBar);

    viewMenu->addAction(toolBar->toggleViewAction());
}

// Subclassing the close event to make verification on the file state before closing application
void MainWindow::closeEvent (QCloseEvent *event)
{
    // Check if file is opened and modified but not saved, then ask user if he really want to close app, if yes close, if not leave unchanged
    if (isOpened == true && isModified == true && isSaved == false)
    {
        int ret = QMessageBox::question(this, "Changes not saved", "The current file changes have not been saved.\n"
                                        "Do you really want to close the application?");
        if (ret == QMessageBox::Yes)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->accept();
    }
}

// Show some application infos
void MainWindow::aboutSlot()
{
   QMessageBox::information(this, "About", "<html>This program has been designed to edit Bethesda Arena TEMPLATE.DAT file "
                            "without the character number limitation. The offset of each text entry is updated in POINTER1.DAT.<br>"
                            "Also keep in mind to not use #, & and % there are special caracters for Arena.<br><br>"
                            "Application created by Dysperia under Qt 5 with QtCreator 3<br><br>"
                            "If you find any bug, please, send an french/english email to<br>"
                            "<font color = #9C00FF>softwatermermaid@hotmail.fr</font>");
}

// Open TEMPLATE.DAT and POINTER1.DAT and build the offset list and entry map
// saveReload is here only to use the function to reload saved template and pointer without the different user ask
void MainWindow::openTemplateSlot(bool saveReload)
{
    bool reloading (false);
    // Check if a duo template and pointer file is already opened and ask user for reloading
    if (isOpened && !saveReload)
    {
        int ret = QMessageBox::question(this, "Open TEMPLATE.DAT", "A session is already running.\n"
                                        "If you load a new TEMPLATE.DAT, unsaved changes of the previous file will be discarded.\n"
                                        "Do you really want to open a new TEMPLATE.DAT file?");
        if (ret == QMessageBox::Yes)
        {
            reloading = true;
        }
    }
    else if (saveReload)
    {
        reloading = true;
    }
    // If no file already opened or user want to open other files
    if (!isOpened || reloading == true)
    {
        if (!saveReload)
        {
            // Ask user directory
            templateDir = QFileDialog::getExistingDirectory(this, "Open directory containing TEMPLATE.DAT and POINTER1.DAT", QDir::homePath());
        }
        // Check if directory not an empty string (user canceled operation...)
        if (templateDir != NULL)
        {
            // Clearing in case of a reloading
            if (reloading == true)
            {
                clearing = true;
                entryEditor->clear();
                entryList->clear();
                entryNameList.clear();
                entryTextList.clear();
                if (!saveReload)
                {
                    templateStream.close();
                    pointerStream.close();
                }
                isOpened = false;
                isSaved = false;
                isModified = false;
                saveTemplate->setDisabled(true);
                clearing = false;
                reloading = false;
            }
            // Building path to files and open the streams
            if (!saveReload)
            {
                templatePath = templateDir.toStdString() + "/TEMPLATE.DAT";
                pointerPath = templateDir.toStdString() + "/POINTER1.DAT";
            }
            templateStream.open(templatePath.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
            pointerStream.open(pointerPath.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
            // If both files are opened
            if (templateStream.is_open() && pointerStream.is_open())
            {
                // offset of TEMPLATE.DAT are written in POINTER1.DAT starting at position 0x2710
                pointerStream.seekg(0x2710, std::ios_base::beg);
                // Vector to store offset of all template texts
                std::vector<uint32_t> templateOffset;
                uint32_t offset;
                pointerStream.read(reinterpret_cast<char*>(&offset), 4);
                // Building the offset vector (after last offset there are some 0 in file (probably to store additional texts if bethesda wanted to)
                while (offset != 0)
                {
                    templateOffset.push_back(offset);
                    pointerStream.read(reinterpret_cast<char*>(&offset), 4);
                }
                // Getting size of TEMPLATE.DAT, used to get the last text size
                templateStream.seekg(0, std::ios_base::end);
                uint32_t templateDatSize = templateStream.tellg();
                templateStream.seekg(0, std::ios_base::beg);
                templateOffset.push_back(templateDatSize);
                // Building the text and text name lists from the offset read previously and text size (while appending \0 to end cstring)
                for (size_t i(0); i<(templateOffset.size()-1); i++)
                {
                    uint32_t entrySize = templateOffset[i+1] - templateOffset[i];
                    char *rawEntry = new char[entrySize+1];
                    templateStream.seekg(templateOffset[i]-1, std::ios_base::beg);
                    templateStream.read(rawEntry, entrySize+1);
                    rawEntry[entrySize] = 0x00;
                    std::string entry(rawEntry);
                    // Text name is followed by \r\n or a space
                    size_t textPos = entry.find_first_of("\r\n ");
                    std::string entryName = entry.substr(0, textPos);
                    entryNameList.append(QString::fromStdString(entryName));
                    entryTextList.append(QString::fromStdString(entry));
                }
                // Building the widget list and setting first entry showed to 0
                // Save is disabled (it is a major problem, any minor change or anything set the text modified in the plainTextEdit,
                // thus, while loading the starting text, is clear and save would normally show, we don't want that)
                clearing = true;
                entryList->addItems(entryNameList);
                entryList->setCurrentRow(0);
                entryEditor->setPlainText(entryTextList[0]);
                clearing = false;
                isOpened = true;
                isModified = false;
                saveTemplate->setDisabled(false);
            }
            // In case A file could not be opened for a reason or an other
            else
            {
                QMessageBox::warning(this, "Open TEMPLATE.DAT/POINTER1.DAT", "TEMPLATE.DAT and/or POINTER1.DAT could not be opened in read/write mode");
            }
        }
    }
}

// Change text in editor when a new name is selected in list
void MainWindow::changeTextSlot(int index)
{
    if (clearing == false && isOpened == true)
    {
        saveTemplate->setDisabled(false);
        entryEditor->setPlainText(entryTextList[index]);
    }
}

// Save the text changes in text list when user edit a text
void MainWindow::saveTextSlot()
{
    if (clearing == false && isOpened == true)
    {
        int index = entryList->currentRow();
        entryTextList[index] = entryEditor->toPlainText();
        isModified = true;
    }
}

// Save all the changes to TEMPLATE.DAT and POINTER1.DAT
void MainWindow::saveTemplateSlot()
{
    // No need to check stream, they are kept running when files are opened at loading
    pointerStream.seekg(0x2710, std::ios_base::beg);
    // For each text, we write to POINTER1.DAT the offset (found with previous text size)
    // Notice that each offset is the position of "#" + 1
    // All text are also written in TEMPLATE.DAT at the same time of course
    // Text are first written to a temp file and then set to replace TEMPLATE.DAT. Both TEMPLATE.DAT and POINTER1.DAT are then reloaded.
    std::ofstream tempofstream;
    std::string tempPath = templateDir.toStdString() + "/TEMPLATE.DAT.TEMP";
    tempofstream.open(tempPath, std::ios_base::out | std::ios_base::binary);
    if (tempofstream.is_open())
    {
        for (size_t i(0); i<size_t(entryTextList.size()); i++)
        {
            uint32_t offset(1);
            for (size_t j(0); j<i; j++)
            {
                offset += entryTextList[j].size();
            }
            pointerStream.write(reinterpret_cast<char*>(&offset), 4);
            // A feature in Qt make all \r\n to become \n. While it is good for general purpose, Arena could not apreciate.
            // First All \r\n are removed to not have some left and then all \n are transform back in \r\n
            QString entry = entryTextList[i];
            entry.replace("\r\n", "\n");
            entry.replace("\n", "\r\n");
            entryTextList[i] = entry;
            tempofstream.write(entry.toStdString().c_str(), entry.size());
        }
        templateStream.close();
        pointerStream.close();
        tempofstream.close();
        QDir templateQDir = QDir(templateDir);
        templateQDir.remove(QString::fromStdString(templatePath));
        templateQDir.rename(QString::fromStdString(tempPath), QString::fromStdString(templatePath));
        // Reloading TEMPLATE.DAT and POINTER1.DAT
        openTemplateSlot(true);
    }
    // Temp file could be created or opened for writting
    else
    {
        QMessageBox::warning(this, "Save TEMPLATE.DAT", "temporary file could not be opened for writting");
    }
}

// View variable list and their meaning (open a popup)
void MainWindow::viewVariableSlot()
{
    QTextEdit *variablePopup = new QTextEdit;
    variablePopup->setText(variablePopupText);
    variablePopup->setMinimumSize(QSize(375, 600));
    variablePopup->show();
}

// Update the variable meaning when a new variable name is selected in the toolbar
void MainWindow::updateVariableSlot(int index)
{
    variableLineEdit->setText(variableMeaning[index]);
}
