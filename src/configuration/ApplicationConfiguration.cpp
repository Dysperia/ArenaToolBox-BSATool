#include <QtCore/QDir>
#include <configuration/ApplicationConfiguration.h>

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

QString ApplicationConfiguration::getLastLoadedArchConf() const {
    return this->value(LAST_LOADED_ARCH_CONF, "ARENA").toString();
}

void ApplicationConfiguration::setLastLoadedArchConf(const QString &lastLoadedArchConf) {
    this->setValue(LAST_LOADED_ARCH_CONF, lastLoadedArchConf);
}
