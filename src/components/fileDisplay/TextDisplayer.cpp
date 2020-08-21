#include "TextDisplayer.h"

//**************************************************************************
// Constructor
//**************************************************************************
TextDisplayer::TextDisplayer(QWidget *parent) : QScrollArea(parent) {
    this->mLabel.setAlignment(Qt::AlignLeft);

    this->setAlignment(Qt::AlignLeft);
    this->setMinimumSize(322, 202);
    this->setWidget(&this->mLabel);
}

//**************************************************************************
// Methods
//**************************************************************************
void TextDisplayer::display(const QString &text) {
    mLabel.setText(text);
    mLabel.adjustSize();
}