#include <assets/FileType.h>
#include <utils/Compression.h>
#include <assets/Dfa.h>
#include "FileDisplayer.h"

//**************************************************************************
// Constructors
//**************************************************************************
FileDisplayer::FileDisplayer(QWidget *parent) : QVBoxLayout(parent) {
    this->defaultDisplayer.setAlignment(Qt::AlignCenter);
    this->defaultDisplayer.setMinimumSize(322, 202);
    this->defaultDisplayer.setText(QString("No preview available for this file."));

    this->mImageDisplayer.setVisible(false);
    this->mTextDisplayer.setVisible(false);
    this->mImagesCollectionDisplayer.setVisible(false);

    this->addWidget(&this->defaultDisplayer);
    this->addWidget(&this->mImageDisplayer);
    this->addWidget(&this->mTextDisplayer);
    this->addWidget(&this->mImagesCollectionDisplayer);
}

//**************************************************************************
// Slots
//**************************************************************************
void FileDisplayer::display(const BsaFile &file, const QVector<char> &fileData) {
    this->defaultDisplayer.setVisible(false);
    this->mImageDisplayer.setVisible(false);
    this->mTextDisplayer.setVisible(false);
    this->mImagesCollectionDisplayer.setVisible(false);

    if (FileType::getExtension(file) == FileType::SET) {
        this->mImageDisplayer.setVisible(true);
        Img img(fileData, 64, static_cast<quint16>(file.size() / 64));
        this->mImageDisplayer.display(img.qImage());
    } else if (FileType::getExtension(file) == FileType::IMG) {
        this->mImageDisplayer.setVisible(true);
        Img img = file.size() == 4096 ? Img(fileData, 64, 64) : Img(fileData);
        this->mImageDisplayer.display(img.qImage());
    } else if (FileType::getExtension(file) == FileType::INF || FileType::getExtension(file) == FileType::TXT) {
        this->mTextDisplayer.setVisible(true);
        QVector<char> decryptedText = Compression::encryptDecrypt(fileData);
        decryptedText.push_back('\0');
        QString text = QString("Encrypted / Decrypted file : \n\n") + decryptedText.data();
        this->mTextDisplayer.display(text);
    } else if (FileType::getExtension(file) == FileType::CIF) {
        this->mImagesCollectionDisplayer.setVisible(true);
        QVector<Img> imgsToDisplay;
        QDataStream stream(QByteArray((fileData.constData()), fileData.size()));
        bool cifValid = true;
        while (!stream.atEnd() && stream.status() == QDataStream::Status::Ok && cifValid) {
            imgsToDisplay.push_back(Img(stream));
            cifValid = !imgsToDisplay.last().qImage().isNull();
        }
        QVector<QImage> framesToDisplay;
        for (const auto& img : imgsToDisplay) {
            framesToDisplay.push_back(img.qImage());
        }
        this->mImagesCollectionDisplayer.display(framesToDisplay);
    } else if (FileType::getExtension(file) == FileType::DFA) {
        this->mImagesCollectionDisplayer.setVisible(true);
        Dfa dfa(fileData);
        this->mImagesCollectionDisplayer.display(dfa.qImages());
    } else {
        this->defaultDisplayer.setVisible(true);
    }
}
