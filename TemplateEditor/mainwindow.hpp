#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QStringList>
#include <fstream>
#include <string>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    // clearing is used to prevent a crash during text update when the comboBox is activated but we're removing all items for example
    bool isOpened, isModified, isSaved, clearing;

    // Declaring 3 string to store paths of template and pointer1 files and base directory
    std::string templatePath, pointerPath;
    QString templateDir;

    // Declaring some widget to modify them later
    QListWidget *entryList;
    QPlainTextEdit *entryEditor;
    QAction *saveTemplate;
    QLineEdit *variableLineEdit;

    // Lists to use with various widgets (in order the comboBox, plainTextEdit, toolbar comboBox and toolbar lineEdit)
    QStringList entryNameList, entryTextList, variableName, variableMeaning;

    // Used in a popup to show all variable and their meaning
    QString variablePopupText;

    // Stream to read and write data from/to TEMPLATE.DAT/POINTER1.DAT
    std::fstream templateStream, pointerStream;

    // Subclassing the close event to make verification on the file state before closing application
    void closeEvent (QCloseEvent *event);

private slots:

    // Show some application infos
    void aboutSlot();

    // Open TEMPLATE.DAT and POINTER1.DAT and build the offset list and entry map
    void openTemplateSlot(bool saveReload = false);

    // Change text in editor when a new name is selected in list
    void changeTextSlot(int index);

    // Save the text changes in the list
    void saveTextSlot();

    // Save all the changes to TEMPLATE.DAT and POINTER1.DAT
    void saveTemplateSlot();

    // View variable list and their meaning
    void viewVariableSlot();

    // Update the variable meaning when a new variable name is selected
    void updateVariableSlot(int index);

public:

    // Constructor
    MainWindow(QWidget *parent = 0);
};

#endif // MAINWINDOW_HPP
