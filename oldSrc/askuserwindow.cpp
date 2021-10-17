#include "askuserwindow.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QString>

// Build popup to ask user info for non native PNG to IMG/SET conversion
AskUserWindow::AskUserWindow(const QString PNGFileName, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Conversion from non native png file");
    QVBoxLayout *askUserLayout = new QVBoxLayout;
    setLayout(askUserLayout);
    QLabel *mainText = new QLabel("The PNG file " + PNGFileName + " matched no Arena native IMG/SET.\n"
                                  "BSATool need some information to create the file.");
    fileExtComboBox = new QComboBox;
    fileExtComboBox->addItem("IMG");
    fileExtComboBox->addItem("SET");
    fileExtComboBox->addItem("Texture (64*64 IMG)");
    QLabel *saveFileType = new QLabel("Save file type");
    QHBoxLayout *fileTypeLayout = new QHBoxLayout;
    fileTypeLayout->addWidget(saveFileType);
    fileTypeLayout->addWidget(fileExtComboBox);
    QLabel *offsetXlabel = new QLabel("Offset X (IMG only)\n0 - 65535");
    QLabel *offsetYlabel = new QLabel("Offset Y (IMG only)\n0 - 65535");
    QHBoxLayout *offsetLabelLayout = new QHBoxLayout;
    offsetLabelLayout->addWidget(offsetXlabel);
    offsetLabelLayout->addWidget(offsetYlabel);
    offsetXbox = new QSpinBox;
    offsetXbox->setRange(0, 65535);
    offsetYbox = new QSpinBox;
    offsetYbox->setRange(0, 65535);
    QHBoxLayout *offsetBoxLayout = new QHBoxLayout;
    offsetBoxLayout->addWidget(offsetXbox);
    offsetBoxLayout->addWidget(offsetYbox);
    settingForAll = new QCheckBox("Use this settings for all other non native files");
    integratePalette = new QCheckBox("Integrate the color palette (IMG only)");
    askUserLayout->addWidget(mainText);
    askUserLayout->addLayout(fileTypeLayout);
    askUserLayout->addLayout(offsetLabelLayout);
    askUserLayout->addLayout(offsetBoxLayout);
    askUserLayout->addWidget(integratePalette);
    askUserLayout->addWidget(settingForAll);
    QPushButton *okButton = new QPushButton("Save file");
    askUserLayout->addWidget(okButton, Qt::AlignCenter);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

// Return file type
QString AskUserWindow::getFileType() const
{
    return fileExtComboBox->currentText();
}

// Return offset X
int AskUserWindow::getOffsetX() const
{
    return offsetXbox->value();
}

// Return offset Y
int AskUserWindow::getOffsetY() const
{
    return offsetYbox->value();
}

// Return integrate palette
bool AskUserWindow::getIntegratePalette() const
{
    return integratePalette->isChecked();
}

// Return setting for all
bool AskUserWindow::getSettingForAll() const
{
    return settingForAll->isChecked();
}
