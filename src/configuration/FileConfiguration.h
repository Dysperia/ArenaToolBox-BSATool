#ifndef BSATOOL_FILECONFIGURATION_H
#define BSATOOL_FILECONFIGURATION_H

#include <QString>
#include <QJsonObject>

/**
 * Configuration of an archived file
 */
class FileConfiguration {
public:

    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief default constructor
     */
    FileConfiguration() = default;

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Read a json object and fill this configuration's fields
     * @param json from which to read the data
     */
    void read(const QJsonObject &json);
    /**
     * Create the json object corresponding to this configuration
     * @param json to which write the data
     */
    void write(QJsonObject &json) const;

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    /**
     * @return the filename of the targeted file
     */
    [[nodiscard]] const QString &getFilename() const;
    /**
     * set the filename of the targeted file
     * @param filename the filename of the targeted file
     */
    void setFilename(const QString &filename);
    /**
     * @return the palette file to use when displaying this file
     */
    [[nodiscard]] const QString &getPaletteFile() const;
    /**
     * set the palette file to use when displaying this file
     * @param paletteFile the palette file to use when displaying this file
     */
    void setPaletteFile(const QString &paletteFile);
    /**
     * @return whether custom palette should be used
     */
    [[nodiscard]] bool isCustomPalette() const;
    /**
     * set whether custom alette should be used
     * @param customPalette whether custom palette should be used
     */
    void setCustomPalette(bool customPalette);
    /**
     * @return the width of the file (need mCustomSize = true)
     */
    [[nodiscard]] quint16 getWidth() const;
    /**
     * set the width of the file (need mCustomSize = true)
     * @param width the width of the file (need mCustomSize = true)
     */
    void setWidth(quint16 width);
    /**
     * @return the height of the file (need mCustomSize = true)
     */
    [[nodiscard]] quint16 getHeight() const;
    /**
     * set the height of the file (need mCustomSize = true)
     * @param height the height of the file (need mCustomSize = true)
     */
    void setHeight(quint16 height);
    /**
     * @return whether custom width and height should be used
     */
    [[nodiscard]] bool isCustomSize() const;
    /**
     * set whether custom width and height should be used
     * @param customSize whether custom width and height should be used
     */
    void setCustomSize(bool customSize);
    /**
     * @return the lack of header in the file
     */
    [[nodiscard]] bool isNoHeader() const;
    /**
     * set the lack of header in the file
     * @param noHeader the lack of header in the file
     */
    void setNoHeader(bool noHeader);

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief filename of the targeted file
     */
    QString mFilename = QString();
    /**
     * @brief palette file to use when displaying this file
     */
    QString mPaletteFile = "";
    /**
     * @brief if custom palette should be used
     */
    bool mCustomPalette = false;
    /**
     * @brief width of the file (need mCustomSize = true)
     */
    quint16 mWidth = 0;
    /**
     * @brief height of the file (need mCustomSize = true)
     */
    quint16 mHeight = 0;
    /**
     * @brief if custom width and height should be used
     */
    bool mCustomSize = false;
    /**
     * @brief the lack of header in the file
     */
    bool mNoHeader = false;
};

#endif // BSATOOL_FILECONFIGURATION_H
