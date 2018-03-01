#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class MenuBar : public QMenuBar
{
public:
    // Constructors
    MenuBar();
    // Getters menus
    QMenu *getFileMenu();
    QMenu *getCancelDeleteUpdateMenu();
    QMenu *getViewMenu();
    QMenu *getExtractMenu();
    QMenu *getExtractAsRawMenu();
    QMenu *getExtractImageMenu();
    QMenu *getPaletteMenu();
    QMenu *getToolMenu();
    QMenu *getAboutMenu();
    // Getters actions
    QAction *getNewBSAFileAction();
    QAction *getOpenBSAFileAction();
    QAction *getSaveBSAFileAction();
    QAction *getAddFileAction();
    QAction *getUpdateFileAction();
    QAction *getDeleteFileAction();
    QAction *getCancelDeleteFileAction();
    QAction *getCancelUpdateFileAction();
    QAction *getQuitAction();
    QAction *getExtendedPreviewAction();
    QAction *getUpdatePreviewOverDefaultAction();
    QAction *getExtractRawFileAction();
    QAction *getExtractRawAllFilteredFilesAction();
    QAction *getExtractDecompressImageAction();
    QAction *getExtractDecompressAllImageAction();
    QAction *getExtractDecompressConvertImageAction();
    QAction *getExtractDecompressConvertAllImageAction();
    QAction *getViewPaletteAction();
    QAction *getEncryptDecryptInfAction();
    QAction *getDecompressExternalIMGAction();
    QAction *getDecompressExternalIMGsToPNGsAction();
    QAction *getConvertPNGsToIMGsSETsAction();
    QAction *getViewExternalIMGAction();
    QAction *getAboutAction();

private:
    // Menus
    QMenu *mFileMenu;
    QMenu *mCancelDeleteUpdateMenu;
    QMenu *mViewMenu;
    QMenu *mExtractMenu;
    QMenu *mExtractAsRawMenu;
    QMenu *mExtractImageMenu;
    QMenu *mPaletteMenu;
    QMenu *mToolMenu;
    QMenu *mAboutMenu;
    // Actions
    QAction *mNewBSAFileAction;
    QAction *mOpenBSAFileAction;
    QAction *mSaveBSAFileAction;
    QAction *mAddFileAction;
    QAction *mUpdateFileAction;
    QAction *mDeleteFileAction;
    QAction *mCancelDeleteFileAction;
    QAction *mCancelUpdateFileAction;
    QAction *mQuitAction;
    QAction *mExtendedPreviewAction;
    QAction *mUpdatePreviewOverDefaultAction;
    QAction *mExtractRawFileAction;
    QAction *mExtractRawAllFilteredFilesAction;
    QAction *mExtractDecompressImageAction;
    QAction *mExtractDecompressAllImageAction;
    QAction *mExtractDecompressConvertImageAction;
    QAction *mExtractDecompressConvertAllImageAction;
    QAction *mViewPaletteAction;
    QAction *mEncryptDecryptInfAction;
    QAction *mDecompressExternalIMGAction;
    QAction *mDecompressExternalIMGsToPNGsAction;
    QAction *mConvertPNGsToIMGsSETsAction;
    QAction *mViewExternalIMGAction;
    QAction *mAboutAction;
};

#endif // MENUBAR_H
