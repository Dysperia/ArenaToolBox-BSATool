#include <assets/FileType.h>
#include <utils/Compression.h>
#include <assets/Dfa.h>
#include <assets/Cfa.h>
#include <components/fileDisplay/FileDisplayer.h>

//**************************************************************************
// Constructors
//**************************************************************************
FileDisplayer::FileDisplayer(QWidget *parent) : QVBoxLayout(parent) {
    this->mDefaultDisplayer.setAlignment(Qt::AlignCenter);
    this->mDefaultDisplayer.setMinimumSize(322, 202);
    this->mDefaultDisplayer.setText(QString("No preview available for this file."));

    this->mImageDisplayer.setVisible(false);
    this->mTextDisplayer.setVisible(false);
    this->mImagesCollectionDisplayer.setVisible(false);
    this->mAnimationDisplayer.setVisible(false);

    this->addWidget(&this->mDefaultDisplayer);
    this->addWidget(&this->mImageDisplayer);
    this->addWidget(&this->mTextDisplayer);
    this->addWidget(&this->mImagesCollectionDisplayer);
    this->addWidget(&this->mAnimationDisplayer);
}

//**************************************************************************
// Slots
//**************************************************************************
void FileDisplayer::display(const BsaFile &file, const QVector<char> &fileData, const ArchiveConfiguration &configuration) {
    mCurrentFile = file;
    this->mDefaultDisplayer.setVisible(false);
    this->mImageDisplayer.setVisible(false);
    this->mTextDisplayer.setVisible(false);
    this->mImagesCollectionDisplayer.setVisible(false);
    this->mAnimationDisplayer.setVisible(false);

    quint16 textureWidth = configuration.getTextureWidth();
    mCurrentPalette = configuration.getDefaultPalette();
    mCurrentPaletteName = configuration.getDefaultPaletteName();
    // Use palette according to file configuration if any
    if (configuration.hasConfigurationForFile(file)) {
        auto &fileConf = configuration.getConfigurationForFile(file);
        if (fileConf.isCustomPalette()) {
            mCurrentPalette = Palette(configuration.getPathForResource(fileConf.getPaletteFile()));
            mCurrentPaletteName = fileConf.getPaletteFile();
        }
    }

    if (FileType::getExtension(file) == FileType::SET) {
        Img img;
        // Load according to configuration if any
        if (configuration.hasConfigurationForFile(file)) {
            auto &fileConf = configuration.getConfigurationForFile(file);
            if (fileConf.isNoHeader() && fileConf.isCustomSize()) {
                img = Img(fileData, fileConf.getWidth(), fileConf.getHeight(), mCurrentPalette);
            }
            else {
                img = Img(fileData, textureWidth, static_cast<quint16>(file.size() / textureWidth), mCurrentPalette);
            }
        } else {
            img = Img(fileData, textureWidth, static_cast<quint16>(file.size() / textureWidth), mCurrentPalette);
        }
        this->mImageDisplayer.setVisible(true);
        this->mImageDisplayer.display(img.qImage());
    } else if (FileType::getExtension(file) == FileType::IMG) {
        Img img;
        // Load according to configuration if any
        if (configuration.hasConfigurationForFile(file)) {
            auto &fileConf = configuration.getConfigurationForFile(file);
            if (fileConf.isNoHeader() && fileConf.isCustomSize()) {
                img = Img(fileData, fileConf.getWidth(), fileConf.getHeight(), mCurrentPalette);
            }
            else {
                img = Img(fileData, mCurrentPalette);
            }
        }
        // special case to load automatically texture without need for a configuration
        else if (file.size() == textureWidth * textureWidth) {
            img = Img(fileData, textureWidth, textureWidth, mCurrentPalette);
        } else {
            img = Img(fileData, mCurrentPalette);
        }
        if (img.hasIntegratedPalette()) {
            mCurrentPalette = img.palette();
            mCurrentPaletteName = "Integrated";
        }
        this->mImageDisplayer.setVisible(true);
        this->mImageDisplayer.display(img.qImage());
    } else if (FileType::getExtension(file) == FileType::INF || FileType::getExtension(file) == FileType::TXT) {
        this->mTextDisplayer.setVisible(true);
        QVector<char> decryptedText = Compression::encryptDecrypt(fileData);
        decryptedText.push_back('\0');
        QString text = QString("Encrypted / Decrypted file : \n\n") + decryptedText.data();
        this->mTextDisplayer.display(text);
    } else if (FileType::getExtension(file) == FileType::CIF) {
        QVector<Img> imgsToDisplay;
        QDataStream stream(QByteArray((fileData.constData()), fileData.size()));
        bool cifValid = true;
        // Load according to configuration if any
        if (configuration.hasConfigurationForFile(file)) {
            auto &fileConf = configuration.getConfigurationForFile(file);
            if (fileConf.isNoHeader() && fileConf.isCustomSize()) {
                while (!stream.atEnd() && stream.status() == QDataStream::Status::Ok && cifValid) {
                    imgsToDisplay.push_back(Img(stream, fileConf.getWidth(), fileConf.getHeight(), mCurrentPalette));
                    cifValid = !imgsToDisplay.last().qImage().isNull();
                }
            } else {
                while (!stream.atEnd() && stream.status() == QDataStream::Status::Ok && cifValid) {
                    imgsToDisplay.push_back(Img(stream, mCurrentPalette));
                    cifValid = !imgsToDisplay.last().qImage().isNull();
                }
            }
        } else {
            while (!stream.atEnd() && stream.status() == QDataStream::Status::Ok && cifValid) {
                imgsToDisplay.push_back(Img(stream, mCurrentPalette));
                cifValid = !imgsToDisplay.last().qImage().isNull();
            }
        }
        QVector<QImage> framesToDisplay;
        for (const auto& img : imgsToDisplay) {
            framesToDisplay.push_back(img.qImage());
        }
        if (!imgsToDisplay.empty() && imgsToDisplay.at(0).hasIntegratedPalette()) {
            mCurrentPalette = imgsToDisplay.at(0).palette();
            mCurrentPaletteName = "Integrated";
        }
        this->mImagesCollectionDisplayer.setVisible(true);
        this->mImagesCollectionDisplayer.display(framesToDisplay);
    } else if (FileType::getExtension(file) == FileType::CFA) {
        this->mAnimationDisplayer.setVisible(true);
        Cfa cfa(fileData, mCurrentPalette);
        this->mAnimationDisplayer.display(cfa.qImages());
    } else if (FileType::getExtension(file) == FileType::DFA) {
        this->mAnimationDisplayer.setVisible(true);
        Dfa dfa(fileData, mCurrentPalette);
        this->mAnimationDisplayer.display(dfa.qImages());
    } else {
        this->mDefaultDisplayer.setVisible(true);
    }
}

//**************************************************************************
// Getters / Setters
//**************************************************************************
const BsaFile &FileDisplayer::getCurrentFile() const {
    return mCurrentFile;
}

const Palette &FileDisplayer::getCurrentPalette() const {
    return mCurrentPalette;
}

const QString &FileDisplayer::getCurrentPaletteName() const {
    return mCurrentPaletteName;
}
