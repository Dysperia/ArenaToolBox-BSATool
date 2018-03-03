#include "MenuBar.h"

//******************************************************************************
// Constructors
//******************************************************************************
MenuBar::MenuBar()
{
    mFileMenu = addMenu("File");
    mNewBSAFileAction = new QAction("New BSA file", this);
    mFileMenu->addAction(mNewBSAFileAction);
    mNewBSAFileAction->setIcon(QIcon("icon/new_bsa.png"));
    mOpenBSAFileAction = new QAction("Open BSA file", this);
    mFileMenu->addAction(mOpenBSAFileAction);
    mOpenBSAFileAction->setIcon(QIcon("icon/open_bsa.png"));
    mSaveBSAFileAction = new QAction("Save BSA file", this);
    mFileMenu->addAction(mSaveBSAFileAction);
    mSaveBSAFileAction->setIcon(QIcon("icon/save_bsa.png"));
    mAddFileAction = new QAction("Add file", this);
    mFileMenu->addAction(mAddFileAction);
    mAddFileAction->setIcon(QIcon("icon/add_file.png"));
    mUpdateFileAction = new QAction("Update file(s)", this);
    mFileMenu->addAction(mUpdateFileAction);
    mUpdateFileAction->setIcon(QIcon("icon/update_file.png"));
    mDeleteFileAction = new QAction("Delete file", this);
    mFileMenu->addAction(mDeleteFileAction);
    mDeleteFileAction->setIcon(QIcon("icon/delete_file.png"));
    mCancelDeleteUpdateMenu = mFileMenu->addMenu("Cancel delete/update file");
    mCancelDeleteFileAction = new QAction("Cancel delete file", this);
    mCancelUpdateFileAction = new QAction("Cancel update file", this);
    mCancelDeleteUpdateMenu->addAction(mCancelUpdateFileAction);
    mCancelDeleteUpdateMenu->addAction(mCancelDeleteFileAction);
    mQuitAction = new QAction("Quit", this);
    mFileMenu->addAction(mQuitAction);
    mQuitAction->setIcon(QIcon("icon/exit.png"));

    mViewMenu = addMenu("View");
    mExtendedPreviewAction = new QAction("Extended preview", this);
    mViewMenu->addAction(mExtendedPreviewAction);
    mUpdatePreviewOverDefaultAction = mViewMenu->addAction("Use update file over default for image preview");
    mUpdatePreviewOverDefaultAction->setCheckable(true);

    mExtractMenu = addMenu("Extract");
    mExtractAsRawMenu = mExtractMenu->addMenu("Extract as raw");
    mExtractRawFileAction = new QAction("Extract file", this);
    mExtractAsRawMenu->addAction(mExtractRawFileAction);
    mExtractRawAllFilteredFilesAction = new QAction("Extract all filtered files", this);
    mExtractAsRawMenu->addAction(mExtractRawAllFilteredFilesAction);
    mExtractImageMenu = mExtractMenu->addMenu("Extract image");
    mExtractDecompressImageAction = new QAction("Extract and decompress IMG/SET", this);
    mExtractImageMenu->addAction(mExtractDecompressImageAction);
    mExtractDecompressAllImageAction = new QAction("Extract and decompress all IMG/SET", this);
    mExtractImageMenu->addAction(mExtractDecompressAllImageAction);
    mExtractDecompressConvertImageAction = new QAction("Extract, decompress and convert IMG/SET to PNG", this);
    mExtractImageMenu->addAction(mExtractDecompressConvertImageAction);
    mExtractDecompressConvertAllImageAction = new QAction("Extract, decompress and convert all IMG/SET to PNG", this);
    mExtractImageMenu->addAction(mExtractDecompressConvertAllImageAction);

    mPaletteMenu = addMenu("Palette");
    mViewPaletteAction = new QAction("View palette", this);
    mPaletteMenu->addAction(mViewPaletteAction);
    mViewPaletteAction->setIcon(QIcon("icon/view_palette.png"));

    mToolMenu = addMenu("Tool");
    mEncryptDecryptInfAction = new QAction("Encrypt/Decrypt INF file", this);
    mToolMenu->addAction(mEncryptDecryptInfAction);
    mDecompressExternalIMGAction = new QAction("Decompress external IMG(s)", this);
    mToolMenu->addAction(mDecompressExternalIMGAction);
    mDecompressExternalIMGsToPNGsAction = new QAction("Decompress and convert external IMG(s)/SET(s) to PNG(s)", this);
    mToolMenu->addAction(mDecompressExternalIMGsToPNGsAction);
    mConvertPNGsToIMGsSETsAction = new QAction("Convert external PNG(s) to IMG/SET", this);
    mToolMenu->addAction(mConvertPNGsToIMGsSETsAction);
    mViewExternalIMGAction = new QAction("View an external IMG/SET", this);
    mToolMenu->addAction(mViewExternalIMGAction);

    mAboutMenu = addMenu("About");
    mAboutAction = new QAction("About", this);
    mAboutMenu->addAction(mAboutAction);
    mAboutAction->setIcon(QIcon("icon/about.png"));
}

//******************************************************************************
// Getters/setters
//******************************************************************************
// Getters menus
QMenu *MenuBar::getFileMenu() {
    return mFileMenu;
}
QMenu *MenuBar::getCancelDeleteUpdateMenu() {
    return mCancelDeleteUpdateMenu;
}
QMenu *MenuBar::getViewMenu() {
    return mViewMenu;
}
QMenu *MenuBar::getExtractMenu() {
    return mExtractMenu;
}
QMenu *MenuBar::getExtractAsRawMenu() {
    return mExtractAsRawMenu;
}
QMenu *MenuBar::getExtractImageMenu() {
    return mExtractImageMenu;
}
QMenu *MenuBar::getPaletteMenu() {
    return mPaletteMenu;
}
QMenu *MenuBar::getToolMenu() {
    return mToolMenu;
}
QMenu *MenuBar::getAboutMenu() {
    return mAboutMenu;
}

// Getters actions
QAction *MenuBar::getNewBSAFileAction() {
    return mNewBSAFileAction;
}
QAction *MenuBar::getOpenBSAFileAction() {
    return mOpenBSAFileAction;
}
QAction *MenuBar::getSaveBSAFileAction() {
    return mSaveBSAFileAction;
}
QAction *MenuBar::getAddFileAction() {
    return mAddFileAction;
}
QAction *MenuBar::getUpdateFileAction() {
    return mUpdateFileAction;
}
QAction *MenuBar::getDeleteFileAction() {
    return mDeleteFileAction;
}
QAction *MenuBar::getCancelDeleteFileAction() {
    return mCancelDeleteFileAction;
}
QAction *MenuBar::getCancelUpdateFileAction() {
    return mCancelUpdateFileAction;
}
QAction *MenuBar::getQuitAction() {
    return mQuitAction;
}
QAction *MenuBar::getExtendedPreviewAction() {
    return mExtendedPreviewAction;
}
QAction *MenuBar::getUpdatePreviewOverDefaultAction() {
    return mUpdatePreviewOverDefaultAction;
}
QAction *MenuBar::getExtractRawFileAction() {
    return mExtractRawFileAction;
}
QAction *MenuBar::getExtractRawAllFilteredFilesAction() {
    return mExtractRawAllFilteredFilesAction;
}
QAction *MenuBar::getExtractDecompressImageAction() {
    return mExtractDecompressImageAction;
}
QAction *MenuBar::getExtractDecompressAllImageAction() {
    return mExtractDecompressAllImageAction;
}
QAction *MenuBar::getExtractDecompressConvertImageAction() {
    return mExtractDecompressConvertImageAction;
}
QAction *MenuBar::getExtractDecompressConvertAllImageAction() {
    return mExtractDecompressConvertAllImageAction;
}
QAction *MenuBar::getViewPaletteAction() {
    return mViewPaletteAction;
}
QAction *MenuBar::getEncryptDecryptInfAction() {
    return mEncryptDecryptInfAction;
}
QAction *MenuBar::getDecompressExternalIMGAction() {
    return mDecompressExternalIMGAction;
}
QAction *MenuBar::getDecompressExternalIMGsToPNGsAction() {
    return mDecompressExternalIMGsToPNGsAction;
}
QAction *MenuBar::getConvertPNGsToIMGsSETsAction() {
    return mConvertPNGsToIMGsSETsAction;
}
QAction *MenuBar::getViewExternalIMGAction() {
    return mViewExternalIMGAction;
}
QAction *MenuBar::getAboutAction() {
    return mAboutAction;
}
