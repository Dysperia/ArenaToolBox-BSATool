#ifndef BSATOOL_FILEDISPLAYER_H
#define BSATOOL_FILEDISPLAYER_H

#include <QVBoxLayout>
#include <configuration/ArchiveConfiguration.h>
#include <components/fileDisplay/ImageDisplayer.h>
#include <bsa/BsaFile.h>
#include <components/fileDisplay/TextDisplayer.h>
#include <components/fileDisplay/ImagesCollectionDisplayer.h>
#include <components/fileDisplay/AnimationDisplayer.h>

/**
 * @brief Class used to display the various file types found in a bsa archive.
 */
class FileDisplayer : public QVBoxLayout {
Q_OBJECT
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief Default constructor
     * @param parent of the FileDisplayer
     */
    explicit FileDisplayer(QWidget *parent = Q_NULLPTR);

    //**************************************************************************
    // Getters / Setters
    //**************************************************************************
    /**
     * @brief Currently displayed file
     */
    [[nodiscard]] const BsaFile &getCurrentFile() const;
    /**
     * @brief Palette of the currently displayed file
     */
    [[nodiscard]] const Palette &getCurrentPalette() const;
    /**
     * @brief Palette name of the currently displayed file
     */
    [[nodiscard]] const QString &getCurrentPaletteName() const;

public slots:

    //**************************************************************************
    // Slots
    //**************************************************************************
    /**
     * Display a visual for the given file
     * @param file to display
     * @param fileData : binary data of the file
     * @param configuration : BSA archive configuration
     */
    void display(const BsaFile &file, const QVector<char> &fileData, const ArchiveConfiguration &configuration);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief Image displayer
     */
    ImageDisplayer mImageDisplayer;
    /**
     * @brief Images collection displayer
     */
    ImagesCollectionDisplayer mImagesCollectionDisplayer;
    /**
     * @brief Animation displayer
     */
    AnimationDisplayer mAnimationDisplayer;
    /**
     * @brief Text file displayer
     */
    TextDisplayer mTextDisplayer;
    /**
     * @brief Default "no content" displayer
     */
    QLabel mDefaultDisplayer;
    /**
     * @brief Currently displayed file
     */
    BsaFile mCurrentFile;
    /**
     * @brief Palette of the currently displayed file
     */
    Palette mCurrentPalette;
    /**
     * @brief Palette name of the currently displayed file
     */
    QString mCurrentPaletteName;
};

#endif // BSATOOL_FILEDISPLAYER_H
