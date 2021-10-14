#include "MenuBar.h"

//******************************************************************************
// Constructors
//******************************************************************************
MenuBar::MenuBar(QWidget *parent): QMenuBar(parent)
{
    // Building menus
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
    mCloseBSAFileAction = new QAction("Close BSA file", this);
    mFileMenu->addAction(mCloseBSAFileAction);
    mCloseBSAFileAction->setIcon(QIcon("icon/close_bsa.png"));

//    mAddFileAction = new QAction("Add file", this);
//    mFileMenu->addAction(mAddFileAction);
//    mAddFileAction->setIcon(QIcon("icon/add_file.png"));
//    mUpdateFileAction = new QAction("Update file(s)", this);
//    mFileMenu->addAction(mUpdateFileAction);
//    mUpdateFileAction->setIcon(QIcon("icon/update_file.png"));
//    mDeleteFileAction = new QAction("Delete file", this);
//    mFileMenu->addAction(mDeleteFileAction);
//    mDeleteFileAction->setIcon(QIcon("icon/delete_file.png"));
//    mCancelDeleteUpdateMenu = mFileMenu->addMenu("Cancel delete/update file");
//    mCancelDeleteFileAction = new QAction("Cancel delete file", this);
//    mCancelUpdateFileAction = new QAction("Cancel update file", this);
//    mCancelDeleteUpdateMenu->addAction(mCancelUpdateFileAction);
//    mCancelDeleteUpdateMenu->addAction(mCancelDeleteFileAction);
//    mQuitAction = new QAction("Quit", this);
//    mFileMenu->addAction(mQuitAction);
//    mQuitAction->setIcon(QIcon("icon/exit.png"));

    mViewMenu = addMenu("View");
//    mExtendedPreviewAction = new QAction("Extended preview", this);
//    mViewMenu->addAction(mExtendedPreviewAction);
//    mUpdatePreviewOverDefaultAction = mViewMenu->addAction("Use update file over default for image preview");
//    mUpdatePreviewOverDefaultAction->setCheckable(true);

    mConfigurationMenu = addMenu("Configuration");

//    mExtractMenu = addMenu("Extract");
//    mExtractAsRawMenu = mExtractMenu->addMenu("Extract as raw");
//    mExtractRawFileAction = new QAction("Extract file", this);
//    mExtractAsRawMenu->addAction(mExtractRawFileAction);
//    mExtractRawAllFilteredFilesAction = new QAction("Extract all filtered files", this);
//    mExtractAsRawMenu->addAction(mExtractRawAllFilteredFilesAction);
//    mExtractImageMenu = mExtractMenu->addMenu("Extract image");
//    mExtractUncompressImageAction = new QAction("Extract and uncompress IMG/SET", this);
//    mExtractImageMenu->addAction(mExtractUncompressImageAction);
//    mExtractUncompressAllImageAction = new QAction("Extract and uncompress all IMG/SET", this);
//    mExtractImageMenu->addAction(mExtractUncompressAllImageAction);
//    mExtractUncompressConvertImageAction = new QAction("Extract, uncompress and convert IMG/SET to PNG", this);
//    mExtractImageMenu->addAction(mExtractUncompressConvertImageAction);
//    mExtractUncompressConvertAllImageAction = new QAction("Extract, uncompress and convert all IMG/SET to PNG", this);
//    mExtractImageMenu->addAction(mExtractUncompressConvertAllImageAction);

//    mPaletteMenu = addMenu("Palette");
//    mViewPaletteAction = new QAction("View palette", this);
//    mPaletteMenu->addAction(mViewPaletteAction);
//    mViewPaletteAction->setIcon(QIcon("icon/view_palette.png"));

//    mToolMenu = addMenu("Tool");
//    mEncryptDecryptInfAction = new QAction("Encrypt/Decrypt INF file", this);
//    mToolMenu->addAction(mEncryptDecryptInfAction);
//    mUncompressExternalIMGAction = new QAction("Uncompress external IMG(s)", this);
//    mToolMenu->addAction(mUncompressExternalIMGAction);
//    mUncompressExternalIMGsToPNGsAction = new QAction("Uncompress and convert external IMG(s)/SET(s) to PNG(s)", this);
//    mToolMenu->addAction(mUncompressExternalIMGsToPNGsAction);
//    mConvertPNGsToIMGsSETsAction = new QAction("Convert external PNG(s) to IMG/SET", this);
//    mToolMenu->addAction(mConvertPNGsToIMGsSETsAction);
//    mViewExternalIMGAction = new QAction("View an external IMG/SET", this);
//    mToolMenu->addAction(mViewExternalIMGAction);

//    mAboutMenu = addMenu("About");
//    mAboutAction = new QAction("About", this);
//    mAboutMenu->addAction(mAboutAction);
//    mAboutAction->setIcon(QIcon("icon/about.png"));
}

//******************************************************************************
// Getters/setters
//******************************************************************************
// Getters menus
QMenu *MenuBar::getFileMenu() {
    return mFileMenu;
}
//QMenu *MenuBar::getCancelDeleteUpdateMenu() {
//    return mCancelDeleteUpdateMenu;
//}
QMenu *MenuBar::getViewMenu() {
    return mViewMenu;
}
//QMenu *MenuBar::getExtractMenu() {
//    return mExtractMenu;
//}
//QMenu *MenuBar::getExtractAsRawMenu() {
//    return mExtractAsRawMenu;
//}
//QMenu *MenuBar::getExtractImageMenu() {
//    return mExtractImageMenu;
//}
//QMenu *MenuBar::getPaletteMenu() {
//    return mPaletteMenu;
//}
//QMenu *MenuBar::getToolMenu() {
//    return mToolMenu;
//}
//QMenu *MenuBar::getAboutMenu() {
//    return mAboutMenu;
//}

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
QAction *MenuBar::getCloseBSAFileAction() {
    return mCloseBSAFileAction;
}
//QAction *MenuBar::getAddFileAction() {
//    return mAddFileAction;
//}
//QAction *MenuBar::getUpdateFileAction() {
//    return mUpdateFileAction;
//}
//QAction *MenuBar::getDeleteFileAction() {
//    return mDeleteFileAction;
//}
//QAction *MenuBar::getCancelDeleteFileAction() {
//    return mCancelDeleteFileAction;
//}
//QAction *MenuBar::getCancelUpdateFileAction() {
//    return mCancelUpdateFileAction;
//}
//QAction *MenuBar::getQuitAction() {
//    return mQuitAction;
//}
//QAction *MenuBar::getExtendedPreviewAction() {
//    return mExtendedPreviewAction;
//}
//QAction *MenuBar::getUpdatePreviewOverDefaultAction() {
//    return mUpdatePreviewOverDefaultAction;
//}
//QAction *MenuBar::getExtractRawFileAction() {
//    return mExtractRawFileAction;
//}
//QAction *MenuBar::getExtractRawAllFilteredFilesAction() {
//    return mExtractRawAllFilteredFilesAction;
//}
//QAction *MenuBar::getExtractUncompressImageAction() {
//    return mExtractUncompressImageAction;
//}
//QAction *MenuBar::getExtractUncompressAllImageAction() {
//    return mExtractUncompressAllImageAction;
//}
//QAction *MenuBar::getExtractUncompressConvertImageAction() {
//    return mExtractUncompressConvertImageAction;
//}
//QAction *MenuBar::getExtractUncompressConvertAllImageAction() {
//    return mExtractUncompressConvertAllImageAction;
//}
//QAction *MenuBar::getViewPaletteAction() {
//    return mViewPaletteAction;
//}
//QAction *MenuBar::getEncryptDecryptInfAction() {
//    return mEncryptDecryptInfAction;
//}
//QAction *MenuBar::getUncompressExternalIMGAction() {
//    return mUncompressExternalIMGAction;
//}
//QAction *MenuBar::getUncompressExternalIMGsToPNGsAction() {
//    return mUncompressExternalIMGsToPNGsAction;
//}
//QAction *MenuBar::getConvertPNGsToIMGsSETsAction() {
//    return mConvertPNGsToIMGsSETsAction;
//}
//QAction *MenuBar::getViewExternalIMGAction() {
//    return mViewExternalIMGAction;
//}
//QAction *MenuBar::getAboutAction() {
//    return mAboutAction;
//}

//**************************************************************************
// Methods
//**************************************************************************
void MenuBar::updateActionsFromBsaArchiveState(bool archiveOpened) {
    if (archiveOpened) {
        mSaveBSAFileAction->setDisabled(false);
        mCloseBSAFileAction->setDisabled(false);
    }
    else {
        mSaveBSAFileAction->setDisabled(true);
        mCloseBSAFileAction->setDisabled(true);
    }
}

void MenuBar::updateConfigurationActions(const QStringList &names, const QString &current) {
    mConfigurationMenu->clear();
    auto actionGroup = new QActionGroup(this);
    for (auto &name : names) {
        auto action = new QAction(name, actionGroup);
        action->setCheckable(true);
        if (name == current) {
            action->setChecked(true);
        }
        actionGroup->addAction(action);
        mConfigurationMenu->addAction(action);
    }
    connect(actionGroup, SIGNAL(triggered(QAction*)), SLOT(emitSelectedConfigurationChanged(QAction*)));
}

void MenuBar::emitSelectedConfigurationChanged(QAction *selected) {
    emit configurationChange(selected->text());
}
