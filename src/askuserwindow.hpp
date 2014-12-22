#ifndef ASKUSERWINDOW_HPP
#define ASKUSERWINDOW_HPP

#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

class AskUserWindow : public QDialog
{
    Q_OBJECT

private:

    QSpinBox *offsetXbox;
    QSpinBox *offsetYbox;
    QCheckBox *settingForAll;
    QCheckBox *integratePalette;

public:

    QComboBox *fileExtComboBox;

    // Build popup to ask user info for non native PNG to IMG/SET conversion
    AskUserWindow(const QString PNGFileName, QWidget *parent = 0);

    // Return file type
    QString getFileType() const;

    // Return offset X
    int getOffsetX() const;

    // Return offset Y
    int getOffsetY()const;

    // Return integrate palette
    bool getIntegratePalette() const;

    // Return setting for all
    bool getSettingForAll() const;
};

#endif // ASKUSERWINDOW_HPP
