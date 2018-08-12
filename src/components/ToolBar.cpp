#include "ToolBar.h"
#include <QToolButton>

//******************************************************************************
// Constructors
//******************************************************************************
ToolBar::ToolBar(MenuBar *menuBar)
{
    setWindowTitle("Toolbar");
    setMovable(false);
    setFloatable(false);
    addAction(menuBar->getNewBSAFileAction());
//    addAction(menuBar->getOpenBSAFileAction());
//    addAction(menuBar->getSaveBSAFileAction());
    addAction(menuBar->getCloseBSAFileAction());
//    addSeparator();
//    addAction(menuBar->getAddFileAction());
//    addAction(menuBar->getDeleteFileAction());
//    addAction(menuBar->getUpdateFileAction());
//    QToolButton *cancelDeleteUpdateButton = new QToolButton;
//    cancelDeleteUpdateButton->setPopupMode(QToolButton::InstantPopup);
//    cancelDeleteUpdateButton->setIcon(QIcon("icon/cancel_deleteUpdate.png"));
//    cancelDeleteUpdateButton->setMenu(menuBar->getCancelDeleteUpdateMenu());
//    addWidget(cancelDeleteUpdateButton);
//    addSeparator();
//    addAction(menuBar->getViewPaletteAction());
//    addSeparator();
//    QToolButton *rawToolButton = new QToolButton;
//    rawToolButton->setPopupMode(QToolButton::InstantPopup);
//    rawToolButton->setIcon(QIcon("icon/raw.png"));
//    rawToolButton->setMenu(menuBar->getExtractAsRawMenu());
//    addWidget(rawToolButton);
//    QToolButton *imageToolButton = new QToolButton;
//    imageToolButton->setPopupMode(QToolButton::InstantPopup);
//    imageToolButton->setIcon(QIcon("icon/image.png"));
//    imageToolButton->setMenu(menuBar->getExtractImageMenu());
//    addWidget(imageToolButton);
//    QToolButton *toolToolButton = new QToolButton;
//    toolToolButton->setPopupMode(QToolButton::InstantPopup);
//    toolToolButton->setIcon(QIcon("icon/tool.png"));
//    toolToolButton->setMenu(menuBar->getToolMenu());
//    addWidget(toolToolButton);
    setIconSize(QSize(36, 36));

    menuBar->getViewMenu()->addAction(toggleViewAction());
}
