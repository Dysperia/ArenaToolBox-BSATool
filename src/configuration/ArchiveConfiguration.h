#ifndef BSATOOL_ARCHIVECONFIGURATION_H
#define BSATOOL_ARCHIVECONFIGURATION_H

#include <QString>
#include <QVector>
#include <configuration/FileConfiguration.h>
#include <QJsonObject>
#include <error/Status.h>
#include <assets/Palette.h>
#include <bsa/BsaFile.h>

/**
 * @brief This class is used to describe and persist an archive configuration
 */
class ArchiveConfiguration {
public:
    //**************************************************************************
    // Statics
    //**************************************************************************

    /**
     * @brief Extension of the archive configuration file
     */
    inline const static QString CONFIGURATION_FILE_EXT = QString(".cfg.bsatool"); // NOLINT(cert-err58-cpp)

    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief default constructor
     */
    ArchiveConfiguration() = default;

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
    /**
     * Read a json object and fill this configuration's fields
     * @param json from which to read the data
     * @return the status of the operation: -1 if failure, 0 if success
     */
    Status loadFromFile();
    /**
     * Create the json object corresponding to this configuration
     * @param json to which write the data
     * @return the status of the operation: -1 if failure, 0 if success
     */
    [[nodiscard]] Status saveToFile() const;

    /**
     * @return true if a configuration exists for the given file, false otherwise
     */
    [[nodiscard]]bool hasConfigurationForFile(const BsaFile &file) const;

    /**
     * @return return the configuration file if exists for the given file
     * @throw Status if not exists
     */
    [[nodiscard]]const FileConfiguration& getConfigurationForFile(const BsaFile &file) const;

    /**
     * @return return the configuration file path for the given filename
     * @param name name of the resource file
     */
    [[nodiscard]]QString getPathForResource(const QString &name) const;

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    /**
     * @return the texture default width
     */
    [[nodiscard]] quint16 getTextureWidth() const;
    /**
     * Set the texture default width
     * @param textureWidth default texture width
     */
    void setTextureWidth(quint16 textureWidth);
    /**
     * @return the filename of default palette
     */
    [[nodiscard]] const QString &getDefaultPaletteName() const;
    /**
     * Set the filename of the default palette
     * @param name name of the default palette file
     */
    void setDefaultPaletteName(const QString &defaultPaletteName);
    /**
     * @return the default palette
     */
    [[nodiscard]] const Palette &getDefaultPalette() const;
    /**
     * @return the name of the default palette
     */
    [[nodiscard]] const QString &getName() const;
    /**
     * Set the name of this configuration
     * @param name path to the files
     */
    void setName(const QString &name);

    /**
     * @return the archive files specific configuration
     */
    QVector<FileConfiguration> &getFiles();

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief texture default width
     */
    quint16 mTextureWidth = 64;
    /**
     * @brief default palette name
     */
    QString mDefaultPaletteName = QString("PAL.COL");
    /**
     * @brief default palette
     */
    Palette mDefaultPalette;
    /**
     * @brief path to configuration files
     */
    QString mName = QStringLiteral("ARENA");
    /**
     * @brief specific configuration for archived files
     */
    QVector<FileConfiguration> mFiles{};
};

#endif // BSATOOL_ARCHIVECONFIGURATION_H
