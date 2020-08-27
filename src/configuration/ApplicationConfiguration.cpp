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
QString ApplicationConfiguration::getLastOpenedBsa() const {
    return this->value(LAST_OPENED_BSA, QDir::homePath()).toString();
}

void ApplicationConfiguration::setLastOpenedBsa(const QString &lastOpenedBsa) {
    this->setValue(LAST_OPENED_BSA, lastOpenedBsa);
}
