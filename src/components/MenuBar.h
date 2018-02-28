#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class MenuBar : public QMenuBar
{
public:
    // Constructors
    MenuBar();
    // Getters menus
    QMenu* getFileMenu();
    QMenu* getCancelDeleteUpdateMenu();
    QMenu* getViewMenu();
    QMenu* getExtractMenu();
    QMenu* getExtractAsRawMenu();
    QMenu* getExtractImageMenu();
    QMenu* getPaletteMenu();
    QMenu* getToolMenu();
    QMenu* getAboutMenu();
    // Getters actions
    QAction* getNewBSAFileAction();
    QAction* getOpenBSAFileAction();
    QAction* getSaveBSAFileAction();
    QAction* getAddFileAction();
    QAction* getUpdateFileAction();
    QAction* getDeleteFileAction();
    QAction* getCancelDeleteFileAction();
    QAction* getCancelUpdateFileAction();
    QAction* getQuitAction();
    QAction* getExtendedPreviewAction();
    QAction* getUpdatePreviewOverDefaultAction();
    QAction* getExtractRawFileAction();
    QAction* getExtractRawAllFilteredFilesAction();
    QAction* getExtractDecompressImageAction();
    QAction* getExtractDecompressAllImageAction();
    QAction* getExtractDecompressConvertImageAction();
    QAction* getExtractDecompressConvertAllImageAction();
    QAction* getViewPaletteAction();
    QAction* getEncryptDecryptInfAction();
    QAction* getDecompressExternalIMGAction();
    QAction* getDecompressExternalIMGsToPNGsAction();
    QAction* getConvertPNGsToIMGsSETsAction();
    QAction* getViewExternalIMGAction();
    QAction* getAboutAction();
private:
    // Menus
    QMenu* fileMenu;
    QMenu* cancelDeleteUpdateMenu;
    QMenu* viewMenu;
    QMenu* extractMenu;
    QMenu* extractAsRawMenu;
    QMenu* extractImageMenu;
    QMenu* paletteMenu;
    QMenu* toolMenu;
    QMenu* aboutMenu;
    // Actions
    QAction* newBSAFileAction;
    QAction* openBSAFileAction;
    QAction* saveBSAFileAction;
    QAction* addFileAction;
    QAction* updateFileAction;
    QAction* deleteFileAction;
    QAction* cancelDeleteFileAction;
    QAction* cancelUpdateFileAction;
    QAction* quitAction;
    QAction* extendedPreviewAction;
    QAction* updatePreviewOverDefaultAction;
    QAction* extractRawFileAction;
    QAction* extractRawAllFilteredFilesAction;
    QAction* extractDecompressImageAction;
    QAction* extractDecompressAllImageAction;
    QAction* extractDecompressConvertImageAction;
    QAction* extractDecompressConvertAllImageAction;
    QAction* viewPaletteAction;
    QAction* encryptDecryptInfAction;
    QAction* decompressExternalIMGAction;
    QAction* decompressExternalIMGsToPNGsAction;
    QAction* convertPNGsToIMGsSETsAction;
    QAction* viewExternalIMGAction;
    QAction* aboutAction;
};

#endif // MENUBAR_H
