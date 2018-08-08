#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MenuBar.h"
#include "../bsa/BsaArchive.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

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
};

#endif // MAINWINDOW_H
