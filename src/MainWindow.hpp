#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <thread>

#include "../globalImage.hpp"
#include "MenuBar.h"

extern Image image;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:

private:
    //=======================================================================================================
    // Extract header under conditions
    void extractHeader();
    void test();
    //=======================================================================================================

    // Contains the last loaded/saved bsa directory
    QString lastBSADirectory;
    // Contains the last open/save file directory
    QString lastOpenSaveFileDirectory;
    // Contains the last open/save col file directory
    QString lastColDirectory;

    // Set to true during clearing only to not allow an info update while clearing
    bool clearing;

    // Set to true if image preview should build updated and not archive file
    bool previewUpdate;

    // Set to true when an animation is played, set to false when not playing or to stop a plying one
    bool isAnimationPlayed;

    // Thread used to play an animation
    std::thread animationThread;

    // Declaring some widget to be modifiabled later
    QComboBox *fileFilter;
    QListWidget *archiveFileList;
    QLabel *previewLabel;
    QLabel *infoLabel;
    MenuBar* menuBar;

    // Qstring containing info on file for use in infoLabel
    QString infoText;

    // QString used in INF file preview
    QString INFContent;

    // List of the different file extensions for filtering
    QStringList FileFilterList;

    // File list for use with the QListWidget when no filter is selected
    QStringList filelist;

    // Map with one QStringList associated with each file extension
    std::map<QString, QStringList> mapFilteredList;

    // Used to store the current preview image filename
    QString previewFileName;

    // Object to store single or multiple image frame data
    std::vector<Image::frameData> imageData;

    // Vector of QPixmap converted from imageData
    std::vector<QPixmap> pixmapList;

    // Generate the file/filter lists
    void generateFileLists();

    // Update showed file list from filter
    void UpdateShowedFileList(const QString &filter);

    // Generate the QString used in the info QLabel
    void generateInfoText(const std::string &fileName);

    // Build preview
    void updatePreview(QListWidgetItem *item);

    // Update the info displayed when a file is selected
    void updateInfoLabel(QListWidgetItem *item);

    // Update the state of QActions according to the current file extension and state (new, updated or delete file)
    void updateQActionsState(QListWidgetItem *item = 0);

    // Clearing function
    void clear();

    // Subclassing the close event to make verification on the archive state before closing application
    void closeEvent (QCloseEvent *event);

    // Play an animation in preview label
    void playAnimation();

private slots:

    // Update the bool previewUpdated according to the user choice
    void setUseUpdatePreviewSlot(bool useUpdate);

    // Open archive file
    void openArchiveSlot();

    // Save settings to file
    void saveSettings();

    // Update window according to the current file extension and state (new, updated or delete file)
    void updateWindowSlot(QListWidgetItem *item);

    // Update file list when a new filter is selected
    void updateFileListFromFilterSlot(QString filter);

    // Extract file
    void extractRawFileSlot();

    // Extract all file visible in filtered list
    void extractRawAllFilteredFilesSlot();

    // Show some application infos
    void aboutSlot();

    // Show the palette used by the current preview image
    void showPreviewPaletteSlot();

    // Extract and decompress IMG/SET
    void extractDecompressImageSlot();

    // Extract and decompress all IMG/SET
    void extractDecompressAllImageSlot();

    // Extract, decompress and convert IMG/SET
    void extractDecompressConvertImageSlot();

    // Extract, decompress and convert All IMG/SET
    void extractDecompressConvertAllImageSlot();

    // Decompress external IMG(s)
    void decompressExternalIMGSlot();

    // View an external IMG/SET file
    void viewExternalIMGSlot();

    // View an extended preview window for the actual IMG/SET in preview
    void viewExtendedPreviewSlot();

    // Decompress and convert external IMGs/SETs to PNG
    void decompressConvertExternalIMGSETSlot();

    // Convert external PNG(s) to IMG(s)
    void convertPNGToIMGSETSlot();

    // Update file(s) in archive data with other(s)
    void updateFileSlot();

    // Delete a file in archive data
    void deleteFileSlot();

    // Add file(s) to archive data
    void addFileSlot();

    // Create a new empty bsa
    void newBsaSlot();

    // Cancel delete file
    void cancelDeleteFileSlot();

    // Cancel update file
    void cancelUpdateFileSlot();

    // Save changes in a BSA archive
    void saveBSASlot();

    // Decrypt or encrypt INF text file
    void encryptDecryptINFSlot();
};

#endif // MAINWINDOW_H
