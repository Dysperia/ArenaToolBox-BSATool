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

private slots:
    // Create a new empty bsa
    void newBsaSlot();
    // Close the opened bsa
    void closeBsaSlot();
};

#endif // MAINWINDOW_H
