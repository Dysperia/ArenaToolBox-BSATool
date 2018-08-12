#ifndef MENUBAR_H
#define MENUBAR_H

#include "../bsa/BsaArchive.h"
#include <QMenuBar>

/**
 * @brief The MenuBar class
 */
class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Default constructor
     *
     * build the menu and the QActions associated with it
     */
    MenuBar(const BsaArchive * const bsaArchive);

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    // Getters menus
    QMenu *getFileMenu();
//    QMenu *getCancelDeleteUpdateMenu();
    QMenu *getViewMenu();
//    QMenu *getExtractMenu();
//    QMenu *getExtractAsRawMenu();
//    QMenu *getExtractImageMenu();
//    QMenu *getPaletteMenu();
//    QMenu *getToolMenu();
//    QMenu *getAboutMenu();
    // Getters actions
    QAction *getNewBSAFileAction();
//    QAction *getOpenBSAFileAction();
//    QAction *getSaveBSAFileAction();
    QAction *getCloseBSAFileAction();
//    QAction *getAddFileAction();
//    QAction *getUpdateFileAction();
//    QAction *getDeleteFileAction();
//    QAction *getCancelDeleteFileAction();
//    QAction *getCancelUpdateFileAction();
//    QAction *getQuitAction();
//    QAction *getExtendedPreviewAction();
//    QAction *getUpdatePreviewOverDefaultAction();
//    QAction *getExtractRawFileAction();
//    QAction *getExtractRawAllFilteredFilesAction();
//    QAction *getExtractDecompressImageAction();
//    QAction *getExtractDecompressAllImageAction();
//    QAction *getExtractDecompressConvertImageAction();
//    QAction *getExtractDecompressConvertAllImageAction();
//    QAction *getViewPaletteAction();
//    QAction *getEncryptDecryptInfAction();
//    QAction *getDecompressExternalIMGAction();
//    QAction *getDecompressExternalIMGsToPNGsAction();
//    QAction *getConvertPNGsToIMGsSETsAction();
//    QAction *getViewExternalIMGAction();
//    QAction *getAboutAction();

public slots:
    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief update the disable state of the bsa releated actions according to
     * the archive state
     * @param archiveOpened the archive opened state
     */
    void updateActionsFromBsaArchiveState(bool archiveOpened);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    // Menus
    QMenu *mFileMenu;
//    QMenu *mCancelDeleteUpdateMenu;
    QMenu *mViewMenu;
//    QMenu *mExtractMenu;
//    QMenu *mExtractAsRawMenu;
//    QMenu *mExtractImageMenu;
//    QMenu *mPaletteMenu;
//    QMenu *mToolMenu;
//    QMenu *mAboutMenu;
    // Actions
    QAction *mNewBSAFileAction;
//    QAction *mOpenBSAFileAction;
//    QAction *mSaveBSAFileAction;
    QAction *mCloseBSAFileAction;
//    QAction *mAddFileAction;
//    QAction *mUpdateFileAction;
//    QAction *mDeleteFileAction;
//    QAction *mCancelDeleteFileAction;
//    QAction *mCancelUpdateFileAction;
//    QAction *mQuitAction;
//    QAction *mExtendedPreviewAction;
//    QAction *mUpdatePreviewOverDefaultAction;
//    QAction *mExtractRawFileAction;
//    QAction *mExtractRawAllFilteredFilesAction;
//    QAction *mExtractDecompressImageAction;
//    QAction *mExtractDecompressAllImageAction;
//    QAction *mExtractDecompressConvertImageAction;
//    QAction *mExtractDecompressConvertAllImageAction;
//    QAction *mViewPaletteAction;
//    QAction *mEncryptDecryptInfAction;
//    QAction *mDecompressExternalIMGAction;
//    QAction *mDecompressExternalIMGsToPNGsAction;
//    QAction *mConvertPNGsToIMGsSETsAction;
//    QAction *mViewExternalIMGAction;
//    QAction *mAboutAction;
};

#endif // MENUBAR_H
