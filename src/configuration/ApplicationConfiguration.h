#ifndef BSATOOL_APPLICATIONCONFIGURATION_H
#define BSATOOL_APPLICATIONCONFIGURATION_H

#include <QtCore/QSettings>

/**
 * @brief This class is used to describe and persist user application settings
 */
class ApplicationConfiguration : public QSettings
{
    Q_OBJECT
public:
    //**************************************************************************
    // statics
    //**************************************************************************
    /**
     * @brief Configuration path to the setting of the last opened bsa
     */
    inline const static QString LAST_OPENED_BSA = QString("lastOpenedBsa"); // NOLINT(cert-err58-cpp)
    inline const static QString LAST_LOADED_ARCH_CONF = QString("lastLoadedArchConf"); // NOLINT(cert-err58-cpp)

    //**************************************************************************
    // Constructor
    //**************************************************************************
    /**
     * @brief default constructor
     */
    ApplicationConfiguration();

    //**************************************************************************
    // Methods
    //**************************************************************************
    /**
     * @brief Return the existing setting or a default value for the path of the last opened bsa
     * @return the last saved path if exists or the path to the home folder
     */
    [[nodiscard]] QString getLastOpenedBsa() const;
    /**
     * @brief Saved the given path as the last opened bsa
     * @param lastOpenedBsa the path to the folder to save in this setting
     */
    void setLastOpenedBsa(const QString &lastOpenedBsa);
    /**
     * @brief Return the existing setting or a default value for the name of the last used archive configuration name
     * @return the last used archive configuration name if exists or ARENA
     */
    [[nodiscard]] QString getLastLoadedArchConf() const;

public slots:
    /**
     * @brief Saved the given string as the last used archive configuration name
     * @param lastLoadedArchConf the confuguration name to save in this setting
     */
    void setLastLoadedArchConf(const QString &lastLoadedArchConf);
};

#endif // BSATOOL_APPLICATIONCONFIGURATION_H
