#include <QtCore/QDir>
#include "ApplicationConfiguration.h"

//**************************************************************************
// Constructor
//**************************************************************************
ApplicationConfiguration::ApplicationConfiguration(): QSettings("ArenaToolBox", "BSATool")
{

}

//**************************************************************************
// Methods
//**************************************************************************
QString ApplicationConfiguration::getLastOpenedBsaFolder() const {
    return this->value(LAST_OPENED_BSA_FOLDER, QDir::homePath()).toString();
}

void ApplicationConfiguration::setLastOpenedBsaFolder(const QString &lastOpenedBsaFolder) {
    this->setValue(LAST_OPENED_BSA_FOLDER, lastOpenedBsaFolder);
}
