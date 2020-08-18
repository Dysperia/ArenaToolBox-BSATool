#ifndef BSATOOL_APPLICATIONCONFIGURATION_H
#define BSATOOL_APPLICATIONCONFIGURATION_H

#include <QtCore/QSettings>

// TODO new class
class ApplicationConfiguration : QSettings
{
public:
    //**************************************************************************
    // statics
    //**************************************************************************
    /**
     * Configuration path to the setting of the last opened bsa folder
     */
    inline const static QString LAST_OPENED_BSA_FOLDER = QString("lastOpenedBsaFolder"); // NOLINT(cert-err58-cpp)

    //**************************************************************************
    // Constructor
    //**************************************************************************
    /**
     * default constructor
     */
    ApplicationConfiguration();

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * Return the existing setting or a default value for the path of the last opened bsa folder
     * @return the last saved path if exists or the path to the home folder
     */
    [[nodiscard]] QString getLastOpenedBsaFolder() const;
    /**
     * Saved the given path as the last opened bsa folder
     * @param lastOpenedBsaFolder the path to the folder to save in this setting
     */
    void setLastOpenedBsaFolder(const QString &lastOpenedBsaFolder);
};

#endif // BSATOOL_APPLICATIONCONFIGURATION_H
