#ifndef BSATOOL_MENUBAR_H
#define BSATOOL_MENUBAR_H

#include <bsa/BsaArchive.h>
#include <QMenuBar>

/**
 * @brief The MenuBar class
 */
class MenuBar : public QMenuBar
{
    Q_OBJECT
signals:
    /**
     * @brief signal sent when a configuration is selected
     * @param name name of the selected configuration;
     */
    void configurationChange(QString name);
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Default constructor
     *
     * build the menu and the QActions associated with it
     */
    explicit MenuBar(QWidget *parent = Q_NULLPTR);

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
    QAction *getOpenBSAFileAction();
    QAction *getSaveBSAFileAction();
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
//    QAction *getExtractUncompressImageAction();
//    QAction *getExtractUncompressAllImageAction();
//    QAction *getExtractUncompressConvertImageAction();
//    QAction *getExtractUncompressConvertAllImageAction();
//    QAction *getViewPaletteAction();
//    QAction *getEncryptDecryptInfAction();
//    QAction *getUncompressExternalIMGAction();
//    QAction *getUncompressExternalIMGsToPNGsAction();
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

    /**
     * @brief update the configuration action list according to the given list
     * @param names list of configuration
     * @param current currently loaded configuration
     */
    void updateConfigurationActions(const QStringList &names, const QString &current);

    /**
     * @brief emit a signal to notify a change in the currently selected configuration
     * @param selected the selected action in the configuration group
     */
    void emitSelectedConfigurationChanged(QAction *selected);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    // Menus
    QMenu *mFileMenu;
//    QMenu *mCancelDeleteUpdateMenu;
    QMenu *mViewMenu;
    QMenu *mConfigurationMenu;
//    QMenu *mExtractMenu;
//    QMenu *mExtractAsRawMenu;
//    QMenu *mExtractImageMenu;
//    QMenu *mPaletteMenu;
//    QMenu *mToolMenu;
//    QMenu *mAboutMenu;
    // Actions
    QAction *mNewBSAFileAction;
    QAction *mOpenBSAFileAction;
    QAction *mSaveBSAFileAction;
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
//    QAction *mExtractUncompressImageAction;
//    QAction *mExtractUncompressAllImageAction;
//    QAction *mExtractUncompressConvertImageAction;
//    QAction *mExtractUncompressConvertAllImageAction;
//    QAction *mViewPaletteAction;
//    QAction *mEncryptDecryptInfAction;
//    QAction *mUncompressExternalIMGAction;
//    QAction *mUncompressExternalIMGsToPNGsAction;
//    QAction *mConvertPNGsToIMGsSETsAction;
//    QAction *mViewExternalIMGAction;
//    QAction *mAboutAction;
};

#endif // BSATOOL_MENUBAR_H
