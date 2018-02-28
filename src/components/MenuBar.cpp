#include "MenuBar.h"

// Constructors
MenuBar::MenuBar()
{
    fileMenu = addMenu("File");
    newBSAFileAction = new QAction("New BSA file", this);
    fileMenu->addAction(newBSAFileAction);
    newBSAFileAction->setIcon(QIcon("icon/new_bsa.png"));
    openBSAFileAction = new QAction("Open BSA file", this);
    fileMenu->addAction(openBSAFileAction);
    openBSAFileAction->setIcon(QIcon("icon/open_bsa.png"));
    saveBSAFileAction = new QAction("Save BSA file", this);
    fileMenu->addAction(saveBSAFileAction);
    saveBSAFileAction->setIcon(QIcon("icon/save_bsa.png"));
    addFileAction = new QAction("Add file", this);
    fileMenu->addAction(addFileAction);
    addFileAction->setIcon(QIcon("icon/add_file.png"));
    updateFileAction = new QAction("Update file(s)", this);
    fileMenu->addAction(updateFileAction);
    updateFileAction->setIcon(QIcon("icon/update_file.png"));
    deleteFileAction = new QAction("Delete file", this);
    fileMenu->addAction(deleteFileAction);
    deleteFileAction->setIcon(QIcon("icon/delete_file.png"));
    cancelDeleteUpdateMenu = fileMenu->addMenu("Cancel delete/update file");
    cancelDeleteFileAction = new QAction("Cancel delete file", this);
    cancelUpdateFileAction = new QAction("Cancel update file", this);
    cancelDeleteUpdateMenu->addAction(cancelUpdateFileAction);
    cancelDeleteUpdateMenu->addAction(cancelDeleteFileAction);
    quitAction = new QAction("Quit", this);
    fileMenu->addAction(quitAction);
    quitAction->setIcon(QIcon("icon/exit.png"));

    viewMenu = addMenu("View");
    extendedPreviewAction = new QAction("Extended preview", this);
    viewMenu->addAction(extendedPreviewAction);
    updatePreviewOverDefaultAction = viewMenu->addAction("Use update file over default for image preview");
    updatePreviewOverDefaultAction->setCheckable(true);

    extractMenu = addMenu("Extract");
    extractAsRawMenu = extractMenu->addMenu("Extract as raw");
    extractRawFileAction = new QAction("Extract file", this);
    extractAsRawMenu->addAction(extractRawFileAction);
    extractRawAllFilteredFilesAction = new QAction("Extract all filtered files", this);
    extractAsRawMenu->addAction(extractRawAllFilteredFilesAction);
    extractImageMenu = extractMenu->addMenu("Extract image");
    extractDecompressImageAction = new QAction("Extract and decompress IMG/SET", this);
    extractImageMenu->addAction(extractDecompressImageAction);
    extractDecompressAllImageAction = new QAction("Extract and decompress all IMG/SET", this);
    extractImageMenu->addAction(extractDecompressAllImageAction);
    extractDecompressConvertImageAction = new QAction("Extract, decompress and convert IMG/SET to PNG", this);
    extractImageMenu->addAction(extractDecompressConvertImageAction);
    extractDecompressConvertAllImageAction = new QAction("Extract, decompress and convert all IMG/SET to PNG", this);
    extractImageMenu->addAction(extractDecompressConvertAllImageAction);

    paletteMenu = addMenu("Palette");
    viewPaletteAction = new QAction("View palette", this);
    paletteMenu->addAction(viewPaletteAction);
    viewPaletteAction->setIcon(QIcon("icon/view_palette.png"));

    toolMenu = addMenu("Tool");
    encryptDecryptInfAction = new QAction("Encrypt/Decrypt INF file", this);
    toolMenu->addAction(encryptDecryptInfAction);
    decompressExternalIMGAction = new QAction("Decompress external IMG(s)", this);
    toolMenu->addAction(decompressExternalIMGAction);
    decompressExternalIMGsToPNGsAction = new QAction("Decompress and convert external IMG(s)/SET(s) to PNG(s)", this);
    toolMenu->addAction(decompressExternalIMGsToPNGsAction);
    convertPNGsToIMGsSETsAction = new QAction("Convert external PNG(s) to IMG/SET", this);
    toolMenu->addAction(convertPNGsToIMGsSETsAction);
    viewExternalIMGAction = new QAction("View an external IMG/SET", this);
    toolMenu->addAction(viewExternalIMGAction);

    aboutMenu = addMenu("About");
    aboutAction = new QAction("About", this);
    aboutMenu->addAction(aboutAction);
    aboutAction->setIcon(QIcon("icon/about.png"));
}

// Getters menus
QMenu* MenuBar::getFileMenu() {
    return fileMenu;
}
QMenu* MenuBar::getCancelDeleteUpdateMenu() {
    return cancelDeleteUpdateMenu;
}
QMenu* MenuBar::getViewMenu() {
    return viewMenu;
}
QMenu* MenuBar::getExtractMenu() {
    return extractMenu;
}
QMenu* MenuBar::getExtractAsRawMenu() {
    return extractAsRawMenu;
}
QMenu* MenuBar::getExtractImageMenu() {
    return extractImageMenu;
}
QMenu* MenuBar::getPaletteMenu() {
    return paletteMenu;
}
QMenu* MenuBar::getToolMenu() {
    return toolMenu;
}
QMenu* MenuBar::getAboutMenu() {
    return aboutMenu;
}

// Getters actions
QAction* MenuBar::getNewBSAFileAction() {
    return newBSAFileAction;
}
QAction* MenuBar::getOpenBSAFileAction() {
    return openBSAFileAction;
}
QAction* MenuBar::getSaveBSAFileAction() {
    return saveBSAFileAction;
}
QAction* MenuBar::getAddFileAction() {
    return addFileAction;
}
QAction* MenuBar::getUpdateFileAction() {
    return updateFileAction;
}
QAction* MenuBar::getDeleteFileAction() {
    return deleteFileAction;
}
QAction* MenuBar::getCancelDeleteFileAction() {
    return cancelDeleteFileAction;
}
QAction* MenuBar::getCancelUpdateFileAction() {
    return cancelUpdateFileAction;
}
QAction* MenuBar::getQuitAction() {
    return quitAction;
}
QAction* MenuBar::getExtendedPreviewAction() {
    return extendedPreviewAction;
}
QAction* MenuBar::getUpdatePreviewOverDefaultAction() {
    return updatePreviewOverDefaultAction;
}
QAction* MenuBar::getExtractRawFileAction() {
    return extractRawFileAction;
}
QAction* MenuBar::getExtractRawAllFilteredFilesAction() {
    return extractRawAllFilteredFilesAction;
}
QAction* MenuBar::getExtractDecompressImageAction() {
    return extractDecompressImageAction;
}
QAction* MenuBar::getExtractDecompressAllImageAction() {
    return extractDecompressAllImageAction;
}
QAction* MenuBar::getExtractDecompressConvertImageAction() {
    return extractDecompressConvertImageAction;
}
QAction* MenuBar::getExtractDecompressConvertAllImageAction() {
    return extractDecompressConvertAllImageAction;
}
QAction* MenuBar::getViewPaletteAction() {
    return viewPaletteAction;
}
QAction* MenuBar::getEncryptDecryptInfAction() {
    return encryptDecryptInfAction;
}
QAction* MenuBar::getDecompressExternalIMGAction() {
    return decompressExternalIMGAction;
}
QAction* MenuBar::getDecompressExternalIMGsToPNGsAction() {
    return decompressExternalIMGsToPNGsAction;
}
QAction* MenuBar::getConvertPNGsToIMGsSETsAction() {
    return convertPNGsToIMGsSETsAction;
}
QAction* MenuBar::getViewExternalIMGAction() {
    return viewExternalIMGAction;
}
QAction* MenuBar::getAboutAction() {
    return aboutAction;
}
