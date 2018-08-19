#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MenuBar.h"
#include "../bsa/BsaArchive.h"

#include <QMainWindow>

/**
 * @brief the main window of the application, used to explore and edit the bsa
 * archive files
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:

public:
    /**
     * @brief MainWindow
     * @param parent parent of this window
     */
    explicit MainWindow(QWidget *parent = nullptr);

public slots:

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief custom menu bar of the application
     */
    MenuBar* mMenuBar;

    /**
     * @brief bsa archive manager
     */
    BsaArchive mBsaArchive;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief start a dialog to ask the user confirmation for opening a archive
     */
    void askUserToConfirmClosingOfOpenedBsa();

private slots:
    /**
     * @brief create a new bsa archive
     */
    void newBsa();

    /**
     * @brief open a existing bsa archive from a user chosen file
     */
    void openBsa();

    /**
     * @brief save the bsa archive to a user chosen file
     */
    void saveBsa();

    /**
     * @brief close the bsa archive
     */
    void closeBsa();
};

#endif // MAINWINDOW_H
