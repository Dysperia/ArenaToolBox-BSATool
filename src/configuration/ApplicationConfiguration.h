#ifndef BSATOOL_APPLICATIONCONFIGURATION_H
#define BSATOOL_APPLICATIONCONFIGURATION_H

#include <QtCore/QSettings>

/**
 * This class is used to describe and persist user application settings
 */
class ApplicationConfiguration : QSettings
{
public:
    //**************************************************************************
    // statics
    //**************************************************************************
    /**
     * Configuration path to the setting of the last opened bsa
     */
    inline const static QString LAST_OPENED_BSA = QString("lastOpenedBsa"); // NOLINT(cert-err58-cpp)

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
     * Return the existing setting or a default value for the path of the last opened bsa
     * @return the last saved path if exists or the path to the home folder
     */
    [[nodiscard]] QString getLastOpenedBsa() const;
    /**
     * Saved the given path as the last opened bsa
     * @param lastOpenedBsa the path to the folder to save in this setting
     */
    void setLastOpenedBsa(const QString &lastOpenedBsa);
};

#endif // BSATOOL_APPLICATIONCONFIGURATION_H
