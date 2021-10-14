#ifndef BSATOOL_ARCHIVECONFIGURATIONLOADER_H
#define BSATOOL_ARCHIVECONFIGURATIONLOADER_H

#include <QString>
#include <error/Status.h>
#include "ArchiveConfiguration.h"
#include <QObject>

class ArchiveConfigurationLoader : public QObject {
Q_OBJECT
public:
    //**************************************************************************
    // Constructors
    //**************************************************************************
    /**
     * @brief default constructor
     */
    ArchiveConfigurationLoader();

    //**************************************************************************
    // Getters/setters
    //**************************************************************************
    [[nodiscard]] const ArchiveConfiguration &getCurrent() const;

    [[nodiscard]] const QStringList &getConfigurationList() const;

public slots:
    //**************************************************************************
    // Slots
    //**************************************************************************
    /**
     * Load a configuration from the given name
     * @param name of the configuration
     * @return the status of the operation: -1 if failure, 0 if success
     */
    Status loadConfiguration(const QString &name);

    /**
     * Update existing configuration list
     * @return the status of the operation: -1 if failure, 0 if success
     */
    Status updateConfigurationList();

private:
    //**************************************************************************
    // Attributes
    //**************************************************************************
    /**
     * @brief currently loaded configuration
     */
    ArchiveConfiguration mCurrent;
    /**
     * @brief list of existing configuration
     */
    QStringList mConfigurationList;
    /**
     * @brief default palette
     */
    QString defaultPalette;
};


#endif //BSATOOL_ARCHIVECONFIGURATIONLOADER_H
