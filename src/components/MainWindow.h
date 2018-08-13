#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MenuBar.h"
#include "../bsa/BsaArchive.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:

public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    MenuBar* mMenuBar;
    BsaArchive mBsaArchive;

    //**************************************************************************
    // Methods
    //**************************************************************************
    void askUserToConfirmClosingOfOpenedBsa();

private slots:
    // Create a new empty bsa
    void newBsa();
    // Open a bsa
    void openBsa();
    // Save a bsa
    void saveBsa();
    // Close the opened bsa
    void closeBsa();
};

#endif // MAINWINDOW_H
